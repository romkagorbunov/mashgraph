#include <bits/stdc++.h>

typedef double ld;

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

	TPoint operator - (const TPoint &a, const TPoint &b) const {
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

inline TPoint operator * (ld k, const TPoint &a) const {
	return TPoint(a.x * k, a.y * k, a.z * k);
}

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

    TObject(TColor _color = TColor(), int _specular = 0, ld _reflective = 0) {
        color = _color;
        specular = _specular;
        reflective = _reflective;
    }
	virtual getN(const TRay ray) = 0;
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
};

enum Type_Light {Type_Ambient, Type_Point, Type_Directional};

struct TLight {
	Type_Light type;
	ld intensity;
	TLight(Type_Light _type = Type_Ambient, ld _intensity = 0) {
	    type = _type;
	    intensity = _intensity;
	}
};
