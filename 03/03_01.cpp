#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

struct Rectangle {

    int upper_left_x;
    int upper_left_y;
    int lower_right_x;
    int lower_right_y;
};

int get_coords_of_shared_area (const Rectangle &rect_1, const Rectangle &rect_2, Rectangle &shared_rect) {

    int left    = std::max (rect_1.upper_left_x,  rect_2.upper_left_x);
    int right   = std::min (rect_1.lower_right_x, rect_2.lower_right_x);
    int top     = std::max (rect_1.upper_left_y,  rect_2.upper_left_y);
    int bottom  = std::min (rect_1.lower_right_y, rect_2.lower_right_y);

    int width  = right - left;
    int height = bottom - top;

    if (width < 0 || height < 0) {

        shared_rect = Rectangle{};
        return 0;
    }

    else {

        shared_rect = Rectangle{left, top, right, bottom};
        return width * height;
    }
}

void get_least_bounding_rect (const Rectangle &rect_1, const Rectangle &rect_2, Rectangle &bounding_rect) {

    bounding_rect.upper_left_x   = std::min (rect_1.upper_left_x,  rect_2.upper_left_x);
    bounding_rect.lower_right_x  = std::max (rect_1.lower_right_x, rect_2.lower_right_x);
    bounding_rect.upper_left_y   = std::min (rect_1.upper_left_y,  rect_2.upper_left_y);
    bounding_rect.lower_right_y  = std::max (rect_1.lower_right_y, rect_2.lower_right_y);
}

void get_area_and_bounding (const std::vector<Rectangle> &Rectangles, int &area, Rectangle &bounding_rect) {

    Rectangle shared_rect   = Rectangles[0];

    int flag = 0;

    for (std::size_t i = 1; i < Rectangles.size(); i++) {

        if (!flag) {

            area = get_coords_of_shared_area (shared_rect, Rectangles[i], shared_rect);
            if (!area)   flag = 1;
        }

        get_least_bounding_rect   (bounding_rect, Rectangles[i], bounding_rect);
    }
}

bool match_rect (Rectangle &r1, Rectangle &r2) {

    if ((r1.upper_left_x == r2.upper_left_x) && 
        (r1.upper_left_y == r2.upper_left_y) &&
        (r1.lower_right_x == r2.lower_right_x) &&
        (r1.lower_right_y == r2.lower_right_y))     return true;
    else                                            return false;
}

bool test_1 () {

    std::vector<Rectangle>    Rectangles{};
    Rectangles.push_back(Rectangle{0, 0, 4, 4});
    Rectangles.push_back(Rectangle{1, 1, 5, 5});
    Rectangles.push_back(Rectangle{2, 2, 6, 3});

    int area = 0;
    Rectangle bounding_rect = Rectangles[0];
    Rectangle bound_true = Rectangle{0, 0, 6, 5};
    get_area_and_bounding (Rectangles, area, bounding_rect);

    if ((area == 2) && match_rect (bounding_rect, bound_true)) return true;
    else                                                                  return false;
}

bool test_2 () {

    std::vector<Rectangle>    Rectangles{};
    Rectangles.push_back(Rectangle{0, 0, 2, 2});
    Rectangles.push_back(Rectangle{3, 3, 5, 5});
    Rectangles.push_back(Rectangle{6, 0, 8, 2});
    Rectangles.push_back(Rectangle{0, 6, 2, 8});

    int area = 0;
    Rectangle bounding_rect = Rectangles[0];
    Rectangle bound_true = Rectangle{0, 0, 8, 8};
    get_area_and_bounding (Rectangles, area, bounding_rect);

    if ((area == 0) && match_rect (bounding_rect, bound_true)) return true;
    else                                                                  return false;
}

bool test_3 () {

    std::vector<Rectangle>    Rectangles{};
    Rectangles.push_back(Rectangle{1, 1, 5, 5});
    Rectangles.push_back(Rectangle{3, 3, 7, 4});
    Rectangles.push_back(Rectangle{2, 2, 6, 6});

    int area = 0;
    Rectangle bounding_rect = Rectangles[0];
    Rectangle bound_true = Rectangle{1, 1, 7, 6};
    get_area_and_bounding (Rectangles, area, bounding_rect);

    if ((area == 2) && match_rect (bounding_rect, bound_true)) return true;
    else                                                                  return false;
}

bool tests () {

    return (test_1 () && test_2 () && test_3 ());
}

int main () {

    assert (tests ());

    return 0;
}