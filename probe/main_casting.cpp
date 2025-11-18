#include <vector>
#include <memory>
struct IS { };
struct S1 : IS { };
struct S2 : IS { };

void process(std::vector<std::unique_ptr<IS>> iss)
{

}

int main()
{
    std::vector<std::unique_ptr<S1>> is;
    is.push_back(std::make_unique<S1>());
    is.push_back(std::make_unique<S1>());
    process(is);
}