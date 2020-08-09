use winapi::shared::windef::POINT;

pub struct Point {
    pub x: i32,
    pub y: i32,
}

impl Point {
    pub fn new(x: i32, y: i32) -> Self {
        Point { x, y }
    }
}

impl From<POINT> for Point {
    fn from(value: POINT) -> Self {
        Self {
            x: value.x,
            y: value.y,
        }
    }
}

impl From<Point> for POINT {
    fn from(value: Point) -> Self {
        Self {
            x: value.x,
            y: value.y,
        }
    }
}
