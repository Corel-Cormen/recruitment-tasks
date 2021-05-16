// *************** UWAGI DO ZADANIA ******************************************
// 1. Jeśli miałbym zaproponować inne podejście do zadania to zaproponowałbym
// stworzenie klasy abstrakcyjnej z funkcją wirtualną i w klasach pochodnych
// nadpisanie tej funkcji.
// 2. Można zoptymalizować szukanie punktu poprzez zastosowanie np. algorytmu
// ekspansji i kontrakcji.
// ***************************************************************************
#include <iostream>
#include <math.h>

#define EPS 0.01        // search speed and precision

using namespace std;

struct vec3 { float x,y,z; };

template <typename Shape>
vec3 support(vec3 v, Shape s)
{
    return s.inflectionPoint(v);
}


struct Sphere
{
     vec3 center;
     float radius;

     vec3 inflectionPoint(vec3 v)
     {
         if(v.x == 0 && v.y == 0 && v.z == 0)
             throw std::invalid_argument("vector zero passed as argument");

         vec3 maxPoint = center;

         // search point
         while(std::abs(maxPoint.x) < std::abs(center.x+radius) &&
               std::abs(maxPoint.y) < std::abs(center.y+radius) &&
               std::abs(maxPoint.z) < std::abs(center.z+radius))
         {
             maxPoint.x += v.x*EPS;
             maxPoint.y += v.y*EPS;
             maxPoint.z += v.z*EPS;
         }

         return maxPoint;
     }

};

// Axis-aligned bounding box. Cube with edges aligned to x,y,z axis.
struct Aabb
{
     vec3 min;
     vec3 max;

     vec3 inflectionPoint(vec3 v)
     {
         if(v.x == 0 && v.y == 0 && v.z == 0)
             throw std::invalid_argument("vector zero passed as argument");

         // center in cube
         vec3 maxPoint;
         maxPoint.x = (min.x + max.x) / 2.0f;
         maxPoint.y = (min.y + max.y) / 2.0f;
         maxPoint.z = (min.z + max.z) / 2.0f;

         // search point
         while((maxPoint.x < max.x && maxPoint.x > min.x) &&
               (maxPoint.y < max.y && maxPoint.y > min.y) &&
               (maxPoint.z < max.z && maxPoint.z > min.z))
         {
             maxPoint.x += v.x*EPS;
             maxPoint.y += v.y*EPS;
             maxPoint.z += v.z*EPS;
         }

         return maxPoint;
     }
};

// Triangular pyramid.
struct Tetrahedron
{
     vec3 points[4];

     vec3 inflectionPoint(vec3 v)
     {
         // point between points[0] and points[1]
         vec3 point_1_2 = { (points[0].x + points[1].x) / 2.0f,
                            (points[0].y + points[1].y) / 2.0f,
                            (points[0].z + points[1].z) / 2.0f };

         // center of gravity in base triangle
         vec3 centerTriangle;
         centerTriangle.x = (points[2].x + point_1_2.x)/3.0f;
         centerTriangle.y = (points[2].y + point_1_2.y)/3.0f;
         centerTriangle.z = (points[2].z + point_1_2.z)/3.0f;

         // center of gravity in tetrahedron
         vec3 centerTetrahedron;
         centerTetrahedron.x = (centerTriangle.x + points[3].x)/3.0f;
         centerTetrahedron.y = (centerTriangle.y + points[3].y)/3.0f;
         centerTetrahedron.z = (centerTriangle.z + points[3].z)/3.0f;

         // distance between vertex of triangle and center of gravity in tetrahedron
         float radius = std::sqrt(std::pow(centerTetrahedron.x - points[0].x, 2) +
                 std::pow(centerTetrahedron.y - points[0].y, 2) +
                 std::pow(centerTetrahedron.z - points[0].z, 2));

         // described triangle in sphere
         Sphere triangleInSphere;
         triangleInSphere.center = centerTetrahedron;
         triangleInSphere.radius = radius;

         return triangleInSphere.inflectionPoint(v);
     }
};

// Cylinder with hemispherical ends.
struct Capsule
{
     vec3 points[2];
     float radius;

     vec3 inflectionPoint(vec3 v)
     {
         // vertices of a cube
         vec3 p1;
         p1.x = points[0].x + radius;
         p1.y = points[0].y + radius;
         p1.z = points[0].z + radius;
         vec3 p2;
         p2.x = points[1].x + radius;
         p2.y = points[1].y + radius;
         p2.z = points[1].z + radius;

         // described capsule in cube
         Aabb cube;
         cube.min = p1;
         cube.max = p2;

         return cube.inflectionPoint(v);
     }
};

int main()
{
    // ---- test Tetrahedron ----
    vec3 vec1 = {1, 1, 0};
    Tetrahedron tetrahedron;
    tetrahedron.points[0] = {0, 0, 0};
    tetrahedron.points[1] = {0, 2, 0};
    tetrahedron.points[2] = {2, 0, 0};
    tetrahedron.points[3] = {1, 1, 2};
    cout << "Tetrahedron\n";
    try {
        vec3 p3 = support(vec1, tetrahedron);
        cout << "x = " << p3.x << " y = " << p3.y << " z = " << p3.z << endl;
    } catch (std::invalid_argument &e) {
        cerr << e.what() << endl;
    }


    // ---- test Capsule ----
    vec3 vec2 = {1, 1, 1};
    Capsule capsule;
    capsule.points[0] = { 0, 0, 0 };
    capsule.points[1] = { 10, 10, 10 };
    capsule.radius = 2;

    cout << "Capsule\n";
    try {
        vec3 p4 = support(vec2, capsule);
        cout << "x = " << p4.x << " y = " << p4.y << " z = " << p4.z << endl;
    } catch(std::invalid_argument &e) {
        cerr << e.what() << endl;
    }

    return 0;
}
