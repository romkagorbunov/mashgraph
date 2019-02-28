#include "geom.h"

extern vector<TLight> lights;
extern vector<TObject*> objects;

ld ComputeLight(TPoint P, TVector N, TPoint V, ld s) {
    ld answer = 0;
    TVector L;
    for (TLight l : lights) {
        if (l.type == Ambient) {
            answer += l.intensity;
            continue;
        } else if (l.type == Point) {
            L = l.pos_dir - P;
        } else if (l.type == Directional) {
            L = l.pos_dir * inf / 4;
        }

        pair<TObject*, ld> cur = Closest(TRay(P, L), 1e-3, 1);
        if (cur.first != NULL) {
            continue;
        }

        ld nl = (N ^ L);
        if (nl > 0) {
            answer += l.intensity * nl / N.len() / L.len();
        }

        if (s != -1) {
            TPoint R = N * 2 * (N ^ L) - L;
            ld rv = (R ^ V);
            if (rv > 0) {
                answer += l.intensity * pow(rv / R.len() / V.len(), s);
            }
        }
    }
    return answer;
}

pair<TObject*, ld> Closest(TRay camera, ld t_min, ld t_max) {
    ld closest_distance = inf;
    TObject* closes_object = NULL;
    for (TObject* s : objects) {
        ld cur = s->intersect(camera, t_min, t_max);
        if (t_min <= cur && cur <= t_max && cur <= closest_distance) {
            closest_distance = cur;
            closes_object = s;
        }
    }
    return mp(closes_object, closest_distance);
}

TVector ReflectiveRay(TVector D, TVector N) {
    return N * 2 * (N ^ D) - D;
}

TColor TraceRay(TRay camera, ld t_min, ld t_max, int depth) {
    pair<TObject*, ld> closest_object_and_distance = Closest(camera, t_min, t_max);
    TObject* closest_object = closest_object_and_distance.first;
    ld closest_distance = closest_object_and_distance.second;
    TColor local_color = TColor(0, 0, 0);
    if (closest_object == NULL) {
        return TColor(0, 0, 0);
    }
    TPoint point_on_object = camera.position + camera.direction * closest_distance;
    TVector N = closest_object->getN(camera.position, point_on_object);
    N = N / N.len();
    local_color = closest_object->color * ComputeLight(point_on_object, N, -camera.direction, closest_object->specular);
    if (depth <= 0 || closest_object->reflective <= 0) {
        return local_color;
    }
    TVector R = ReflectiveRay(-camera.direction, N);
    TColor reflevtive_color = TraceRay(TRay(point_on_object, R), 0.001, inf / 2, depth - 1);
    return (local_color * (1 - closest_object->reflective) + reflevtive_color * closest_object->reflective);
}
