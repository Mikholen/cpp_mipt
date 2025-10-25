#include <cmath>
#include <numbers>

class Triangle {

public:
    
    Triangle (double a, double b, double c) 
    : side_1 (a), 
      side_2 (b), 
      side_3 (c) {}

    double perimeter () {

        return side_1 + side_2 + side_3;
    }

    double area () {

        double semi_p = perimeter () / 2;
        double area = std::sqrt (semi_p * (semi_p - side_1) * (semi_p - side_2) * (semi_p - side_3));

        return area;
    }

private:

        double side_1;
        double side_2;
        double side_3;
};

class Square {

public:
    
    Square (double a) : side (a) {}

    double perimeter () {
        
        return side * 4;
    }

    double area () {

        return side * side;
    }

private:

        double side;
};

class Circle {

public:
    
    Circle (double r) : radius (r) {}

    double perimeter () {
        
        return 2 * radius * std::numbers::pi;
    }

    double area () {

        return radius * radius * std::numbers::pi;
    }

private:

        double radius;
};

int main () {

    Triangle    triangle (3, 4, 5);
    Square      square (5);
    Circle      circle (8);

    return 0;
}