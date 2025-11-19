#include <cmath>
#include <numbers>
#include <cassert>
#include <vector>

class Shape {

public:

    virtual ~Shape () = default;

    virtual double area ()      const = 0;
    virtual double perimeter () const = 0;
};

class Triangle : public Shape {

public:
    
    Triangle (double a, double b, double c) 
    : side_1 (a), 
      side_2 (b), 
      side_3 (c) {}

    double perimeter () const override final {

        return side_1 + side_2 + side_3;
    }

    double area () const override final {

        double semi_p = perimeter () / 2;
        double area = std::sqrt (semi_p * (semi_p - side_1) * (semi_p - side_2) * (semi_p - side_3));

        return area;
    }

private:

        double side_1;
        double side_2;
        double side_3;
};

class Square final : public Shape {

public:
    
    Square (double a) : side (a) {}

    double perimeter () const override {
        
        return side * 4;
    }

    double area () const override {

        return side * side;
    }

private:

        double side;
};

class Circle final : public Shape {

public:
    
    Circle (double r) : radius (r) {}

    double perimeter () const override {
        
        return 2 * radius * std::numbers::pi;
    }

    double area () const override {

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

    std::vector < Shape * > triangles;
    triangles.push_back (new Triangle (3, 4, 5));
    triangles.push_back (new Triangle (6, 6, 6));
    triangles.push_back (new Triangle (7, 8, 9));

    if (is_close (triangles[0]->area(), 6.0)    && is_close (triangles[0]->perimeter (), 12.0) &&
        is_close (triangles[1]->area(), 15.588) && is_close (triangles[1]->perimeter (), 18.0) &&
        is_close (triangles[2]->area(), 26.833) && is_close (triangles[2]->perimeter (), 24.0)) {

        for (auto triangle : triangles) {

            delete triangle;
        }
        return true;
    }

    for (auto triangle : triangles) {

        delete triangle;
    }

    return false;
}

bool square_tests () {

    std::vector < Shape * > squares;
    squares.push_back (new Square (6));
    squares.push_back (new Square (1.5));
    squares.push_back (new Square (4.2));

    if (is_close (squares[0]->area(), 36.0)  && is_close (squares[0]->perimeter (), 24.0) &&
        is_close (squares[1]->area(), 2.25)  && is_close (squares[1]->perimeter (), 6.0)  &&
        is_close (squares[2]->area(), 17.64) && is_close (squares[2]->perimeter (), 16.8)) {

        for (auto square : squares) {

            delete square;
        }
        return true;
    }

    return false;
}

bool circle_tests () {

    std::vector < Shape * > circles; 
    circles.push_back (new Circle (2));
    circles.push_back (new Circle (1.5));
    circles.push_back (new Circle (7.8));

    if (is_close (circles[0]->area(), 12.566)  && is_close (circles[0]->perimeter (), 12.566) &&
        is_close (circles[1]->area(), 7.069)   && is_close (circles[1]->perimeter (), 9.425)  &&
        is_close (circles[2]->area(), 191.134) && is_close (circles[2]->perimeter (), 49.009)) {

        for (auto circle : circles) {

            delete circle;
        }
        return true;
    }

    return false;
}

bool tests () {

    return (triangle_tests () && square_tests () && circle_tests ());
}

int main () {

    assert (tests ());

    return 0;
}