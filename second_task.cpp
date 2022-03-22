#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>

// Романов В.А.

// Задача 2.
// Движение манипуляторов это конечно хорошо, но обычно с помощью кода они двигаются по определенным координатам, для сборки того или иного компонента.
// Дано: два манипулятора, которые описываются стартовыми центрами O1 и O2 и радиусом их действия R1 и R2 в декартовой системе координат.
// Радиус действия не изменяется в течении работы программы. Также дан массив точек, где лежат детали P1…Pn.

// Выведете две строки: какие точки обойдет каждый манипулятор
// Имейте в виду,что до детали должен дойти “оптимальный” манипулятор на текущей итерации процесса.
// P.S. Оптимальность считаем по длине до точки. Радиус не учитываем. Пример ниже.


// Двумерный вектор
struct Point {
    float x, y;

    Point( float x, float y) : x(x), y(y) {}


    // Расстояние между двумя точками
    static float distance( const Point first, const Point second ) {
        return sqrtf( (first.x - second.x) * (first.x - second.x) +
                     (first.y - second.y) * (first.y - second.y ) );
    }

    friend std::ostream& operator<<( std::ostream& output, const Point point) {
        output << "{" << point.x << ", " << point.y << "}";
        return output;
    }
};





class Manipulator {
private:
    // Координаты центра
    Point _point;
    // Радиус эффективного действия
    float _r;
public:
    explicit  Manipulator( Point point = Point(0, 0), uint r = 1) : _point(point), _r( r ) {}

    // Гетер
    Point point() const { return _point; }

    void go_to(const Point point) {
        _point = point;
    }

    // расстояние от точки до манипулятора с учетом радиуса действия
    //  output : -r < 0 => Точка внутри радиуса действия
    // output : > r => Точка снаружи радиуса действия
    float req_distance_wth_r(const Point point ) const {
        return Point::distance( _point, point) - _r;
    }

    // расстояние до точки
    float distance(const Point point) const {
        return Point::distance( _point, point);
    }


    // Определение какой манипулятор лучше подходит для точки
    // output: false => Подходит первый
    // output: true => Подходит второй
    static bool cmp_with_r(const Manipulator first, const Manipulator second, const Point point) {
        if (first.req_distance_wth_r(point) > second.req_distance_wth_r(point)) {
            return true;
        } else {
            return false;
        }
    }

    static bool cmp(const Manipulator first, const Manipulator second, const Point point) {
        if (first.distance(point) > second.distance(point)) {
            return true;
        } else {
            return false;
        }
    }
};

// микро unit тесты
void assert( bool eq ) {
    if (!eq) {
        throw std::exception();
    }
}




int main() {
    // проверка distance
    assert( Point::distance(Point(0, 0), Point(3 ,4 )) == 5.0f);

    // проверка Manipulator::req_distance_wth_r

    auto first = Manipulator(Point(0, 0), 5);
    auto second = Manipulator(Point(0, 0), 4);

    assert(first.req_distance_wth_r(Point(3, 4)) == 0.0f);
    assert(second.req_distance_wth_r(Point(10, 0)) == 6.0f);

    // проверка Manipulator::cmp_with_r

    assert(!Manipulator::cmp_with_r(first, second, Point(10, 0))); // false
    assert(Manipulator::cmp_with_r(first, Manipulator(Point(2, 2), 4), Point(4, 4))); // true



    // ЗАДАНИЕ № 2

    std::vector<Point> points = {
            Point(1.0f, 3.0f),
            Point(2.0f, 1.41f),
            Point(0.2f, -7.0f),
            Point(-5.0f, -1.0f),
            Point(0.0f, 9.0f),
    };

    auto m1 = Manipulator(Point(0, 0), 4);
    auto m2 = Manipulator(Point(2, 1), 3);



    std::stringstream f_out;
    std::stringstream s_out;

    for (auto & point : points) {
        if ( !Manipulator::cmp(m1, m2, point) ) { // cmp == false
            m1.go_to(point);

            f_out << m1.point();
            s_out << "\t";
        } else {
            m2.go_to(point);

            f_out << "\t";
            s_out << m2.point();

        }
    }

    f_out << std::endl;
    s_out << std::endl;

    std::string first_out = f_out.str();
    std::string second_out = s_out.str();

    std::cout << "m1: " << first_out;
    std::cout << "m2: " << second_out;

    return 0;
}