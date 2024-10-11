
class Shape
{
public:
  virtual void Area() const = 0;
  virtual ~Shape() {}
};

class Square : Shape
{
private:
  int width, height;
public:
  virtual void Area() const = 0;
};