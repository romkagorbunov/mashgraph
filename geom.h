#include <bits/stdc++.h>
#define mp make_pair

using namespace std;

typedef double ld;

const ld inf = 1e12;

struct TPoint {
	ld x, y, z;
	TPoint(ld X = 0, ld Y = 0, ld Z = 0) {
		x = X;
		y = Y;
		z = Z;
	}

	TPoint operator += (const TPoint &a) {
	    x += a.x;
	    y += a.y;
	    z += a.z;
	    return *this;
	}

    TPoint operator -= (const TPoint &a) {
        x -= a.x;
        y -= a.y;
        z -= a.z;
        return *this;
    }

    TPoint operator *= (ld k) {
        x *= k;
        y *= k;
        z *= k;
        return *this;
	}

	TPoint operator + (const TPoint &b) const {
		return TPoint(x + b.x, y + b.y, z + b.z);
	}

	TPoint operator - (const TPoint &b) const {
		return TPoint(x - b.x, y - b.y, z - b.z);
	}

	TPoint operator * (ld k) const {
		return TPoint(x * k, y * k, z * k);
	}

	TPoint operator / (ld k) const {
		return TPoint(x / k, y / k, z / k);
	}

	TPoint operator - () const {
		return TPoint(-x, -y, -z);
	}

	TPoint operator * (const TPoint &b) const {
		return TPoint(y * b.z - b.y * z, z * b.x - x * b.z, x * b.y - y * b.x);
	}

	ld operator ^ (const TPoint &b) const {
		return x * b.x + y * b.y + z * b.z;
	}

	ld len() const {
		return sqrt(x * x + y * y + z * z);
	}

	ld sqlen() const {
		return x * x + y * y + z * z;
	}
};

/*TPoint operator * (ld k, const TPoint &a) {
	return TPoint(a.x * k, a.y * k, a.z * k);
}*/

typedef TPoint TVector;

struct TColor {
	ld r, g, b;
	TColor(ld R = 0, ld G = 0, ld B = 0) {
		r = R;
		g = G;
		b = B;
	}
	TColor operator + (const TColor &a) {
	    return TColor(r + a.r, g + a.g, b + a.b);
	}
	TColor operator - (const TColor &a) {
	    return TColor(r - a.r, g - a.g, b - a.b);
	}
	TColor operator * (ld k) {
	    return TColor(r * k, g * k, b * k);
	}
	TColor operator / (ld k) {
	    return TColor(r / k, g / k, b / k);
	}
};

struct TRay {
    TPoint position;
    TVector direction;
    TRay(TPoint POSITION = TPoint(), TVector DIRECTION = TPoint()) {
        position = POSITION;
        direction = DIRECTION;
    }
    TRay operator + (const TVector &a) const {
        return TRay(position, direction + a);
    }
    TRay operator * (ld k) const {
        return TRay(position, direction * k);
    }
};

struct TObject {
    TColor color;
    int specular;
    ld reflective;
    ld transparency;

    TObject(TColor _color = TColor(), int _specular = 0, ld _reflective = 0, ld _transparency = 0) {
        color = _color;
        specular = _specular;
        reflective = _reflective;
        transparency = _transparency;
    }
	virtual TVector getN(const TPoint &camera, const TPoint &point_on_object) {
		cerr << "FUCK THIS SHIT" << endl;
    }
    virtual ld intersect(const TRay &camera, ld l, ld r) {
		cerr << "FUCK THIS SHIT" << endl;
    }
};

struct TSphere : TObject {
    TPoint center;
    ld radius;
    TSphere(TPoint _center = TPoint(), ld _radius = 0, TObject _obj = TObject()) {
		center = _center;
		radius = _radius;
		color = _obj.color;
		specular = _obj.specular;
		reflective = _obj.reflective;
	}
	ld intersect(const TRay &camera, ld l, ld r) override {
		TVector oc = camera.position - center;
		ld k1 = (camera.direction ^ camera.direction);
		ld k2 = 2 * (oc ^ camera.direction);
		ld k3 = (oc ^ oc) - (radius * radius);
		ld d = k2 * k2 - 4 * k1 * k3;
		if (d < 0) {
			return inf;
		}
		ld t1 = (-k2 + sqrt(d)) / (2 * k1);
		ld t2 = (-k2 - sqrt(d)) / (2 * k1);
		if (t2 < t1) {
			swap(t1, t2);
		}
		if (l <= t1 && t1 <= r) {
			return t1;
		} else if (l <= t2 && t2 <= r) {
			return t2;
		}
    }
    TVector getN(const TPoint &camera, const TPoint &point_on_sphere) override {
    	//point is the point on sphere, for which we try to find N vector
    	if ((camera - center).sqlen() >= radius * radius) {
    		return (point_on_sphere - center) / (point_on_sphere - center).len();
    	} else {
    		return (center - point_on_sphere) / (center - point_on_sphere).len();
    	}
    }
};

enum Type_Light {Ambient, Point, Directional};

struct TLight {
	Type_Light type;
	ld intensity;
	TPoint pos_dir;
	TLight(Type_Light _type = Ambient, ld _intensity = 0, TPoint _pos_dir = TPoint()) {
	    type = _type;
	    intensity = _intensity;
	    pos_dir = _pos_dir;
	}
};

ld ComputeLight(TPoint P, TVector N, TPoint V, ld s);
pair<TObject*, ld> Closest(TRay camera, ld t_min, ld t_max);
TVector ReflectiveRay(TVector D, TVector N);
TColor TraceRay(TRay camera, ld t_min, ld t_max, int depth);