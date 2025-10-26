#include <cmath>
#include <numbers>
#include <cassert>

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

bool is_close (double num_1, double num_2) {

    if (abs(num_1 - num_2) < 0.01) return true;
    
    return false;
}

bool triangle_tests () {

    Triangle    triangle_1 (3, 4, 5);
    Triangle    triangle_2 (6, 6, 6);
    Triangle    triangle_3 (7, 8, 9);

    if (is_close (triangle_1.area(), 6.0)  && is_close (triangle_1.perimeter (), 12.0)   &&
        is_close (triangle_2.area(), 15.588) && is_close (triangle_2.perimeter (), 18.0) &&
        is_close (triangle_3.area(), 26.833) && is_close (triangle_3.perimeter (), 24.0))  
        
        return true;

    return false;
}

bool square_tests () {

    Square    square_1 (6);
    Square    square_2 (1.5);
    Square    square_3 (4.2);

    if (is_close (square_1.area(), 36.0)  && is_close (square_1.perimeter (), 24.0) &&
        is_close (square_2.area(), 2.25)  && is_close (square_2.perimeter (), 6.0)  &&
        is_close (square_3.area(), 17.64) && is_close (square_3.perimeter (), 16.8))  return true;

    return false;
}

bool circle_tests () {

    Circle    circle_1 (2);
    Circle    circle_2 (1.5);
    Circle    circle_3 (7.8);

    if (is_close (circle_1.area(), 12.566)  && is_close (circle_1.perimeter (), 12.566) &&
        is_close (circle_2.area(), 7.069)   && is_close (circle_2.perimeter (), 9.425)  &&
        is_close (circle_3.area(), 191.134) && is_close (circle_3.perimeter (), 49.009))  return true;

    return false;
}

bool tests () {

    return (triangle_tests () && square_tests () && circle_tests ());
}

int main () {

    assert (tests ());

    return 0;
}