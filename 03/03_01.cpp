#include <cmath>
#include <vector>

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
    int height = top - bottom;

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

    bounding_rect.upper_left_x    = std::min (rect_1.upper_left_x,  rect_2.upper_left_x);
    bounding_rect.lower_right_x  = std::max (rect_1.lower_right_x, rect_2.lower_right_x);
    bounding_rect.upper_left_y   = std::min (rect_1.upper_left_y,  rect_2.upper_left_y);
    bounding_rect.lower_right_y  = std::max (rect_1.lower_right_y, rect_2.lower_right_y);
}

int main () {

    std::vector<Rectangle>    Rectangles{};
    Rectangle shared_rect   = Rectangles[0];
    Rectangle bounding_rect = Rectangles[0];

    int flag = 0;
    int area = 0;

    for (std::size_t i = 1; i < Rectangles.size(); i++) {

        if (!flag) {

            area = get_coords_of_shared_area (shared_rect, Rectangles[i], shared_rect);
            if (area)   flag = 1;
        }

        get_least_bounding_rect   (bounding_rect, Rectangles[i], bounding_rect);
    }

    return 0;
}