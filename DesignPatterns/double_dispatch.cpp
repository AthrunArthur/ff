/////////////
/* This file show how to implement a "double dispatch" pattern in C++.
 The key idea here is mixing compile time and runtime information, as show below,
    struct C
    {
         static int ID;
         static void * class_id(){return &ID;}
         virtual void * cid(){return &ID;}
     };
 Furthermore, we use two classes, RuntimeInfo and CompileTimeInfo, to represent this concept.
 */

#include <iostream>
#include <map>


///////////////////Wrappers or Internal thing.
typedef void * RID_t;

struct RunTimeInfo
{
    virtual RID_t cid() = 0;
};

template<class T>
struct CompileTimeInfo
{
    static int ID;
    static RID_t get_class_id() {return &CompileTimeInfo<T>::ID;}
};

template<class T>
int CompileTimeInfo<T>::ID = 0;

struct internal_key
{
    internal_key(RID_t p1, RID_t p2)
    : key1(p1)
    , key2(p2){}
    RID_t key1;
    RID_t key2;
};


class Shape;
typedef std::map<internal_key,
std::function<double (Shape &, Shape &) >,
std::function<bool (const internal_key &, const internal_key &) > > SSMap_t;

//////////////Example to show how to implement double-dispatch pattern.
class Shape : public RunTimeInfo
{
public:
    template<class S1, class S2>
    static void   add_intersections( std::function<double (S1 &, S2 &)> f)
    {
        s_oIntersectFuncs.insert(std::make_pair(
                                                internal_key(S1::get_class_id(), S2::get_class_id()),
                                                [&f](Shape & p1, Shape & p2){return f(static_cast<S1 &>(p1), static_cast<S2 &>(p2));}));
        s_oIntersectFuncs.insert(std::make_pair(
                                                internal_key(S2::get_class_id(), S1::get_class_id()),
                                                [&f](Shape &p1, Shape & p2){return f(static_cast<S1 &>(p2), static_cast<S2 &>(p1));}));
    }
    
    ///This is the polymorphism thing. Notice it's not a virtual function!
    double intersects(Shape & s)
    {
        return s_oIntersectFuncs.at(internal_key(cid(), s.cid()))(*this, s);
    }
    
protected:
    static SSMap_t s_oIntersectFuncs;
};

SSMap_t Shape:: s_oIntersectFuncs([](const internal_key & k1, const internal_key & k2){
    if(k1.key1 > k2.key1)
        return true;
    else if(k1.key1 == k2.key1 && k1.key2 > k2.key2)
        return true;
    else
        return false;
});

class Rect : public Shape, public CompileTimeInfo<Rect>
{
public:
    virtual void * cid(){return &CompileTimeInfo<Rect>::ID;}
};

class Circle : public Shape, public CompileTimeInfo<Circle>
{
public:
    virtual void * cid(){return &CompileTimeInfo<Circle>::ID;}
};

class Triangle : public Shape, public CompileTimeInfo<Triangle>
{
public:
    virtual void * cid(){return &CompileTimeInfo<Triangle>::ID;}
};

///Notice what will happen if we need another Shape, say eclipse?

double rect_with_circle( Rect & r, Circle & c)
{
    std::cout<<"This is rect with circle"<<std::endl;
    return 1.0;
}
double rect_with_triangle(Rect & r, Triangle & t )
{
    std::cout<<"This is rect with triangle "<<std::endl;
    return 2.0;
}


int main()
{
    Shape::add_intersections<Rect, Circle>(rect_with_circle);
    //Shape::add_intersections<Circle, Rect>(rect_with_circle); //Type error!
    Shape::add_intersections<Rect, Triangle>(rect_with_triangle);
    Rect r1;
    Circle c1;
    Triangle t1;
    r1.intersects(c1);
    c1.intersects(r1);
    r1.intersects(t1);
    //c1.intersects(t1); //Runtime exception! We can use try-catch to know what happens here!
    
    return 0;
}