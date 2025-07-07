#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <iostream>

#include <boost/multi_index/hashed_index.hpp>

#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/array.hpp>

#include <fstream>

using namespace boost;
using namespace boost::multi_index;

struct employee
{
  int         id;
  std::string name;

  employee(int id,const std::string& name):id(id),name(name){}

  bool operator<(const employee& e) const {
    return id < e.id;
  }
  void Print() const {
    std::cout << "Id: " << id << ", name: " << name << "\n";
  }
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & id;
        ar & name;
    }
  bool operator==(const employee &other) const {
    return (id == other.id && name == other.name);
  }
};

template<>
struct std::hash<employee> {
  std::size_t operator()(const employee& k) const {
    return std::hash<int>()(k.id);
  }
};

struct EmpHash1 {
  std::size_t operator()(const employee& k) const {
    return std::hash<int>()(k.id);
  }
};

struct EmpHash2 {
  std::size_t operator()(const employee& k) const {
    return std::hash<std::string>()(k.name);
  }
};

// define a multiply indexed set with indices by id and name
typedef multi_index_container<
  employee,
  indexed_by<
    
    // sort by employee::operator<
    ordered_unique<identity<employee>>,
    
    // sort by less<string> on name
    ordered_non_unique<member<employee,std::string,&employee::name>>,
    
    // hashed sort by hash<id>
    hashed_unique<member<employee,int,&employee::id>>,

    // hashed sort by hash<name>
    hashed_non_unique<member<employee,std::string,&employee::name>>
  > 
> employee_set;

void print_out(const employee_set& es)
{
  std::cout << "get a view to index #0 (id)\n";
  const auto& id_index=es.get<0>();
  for (auto name : id_index) {
    name.Print();
  }

  std::cout << "get a view to index #1 (name)\n";
  const employee_set::nth_index<1>::type& name_index=es.get<1>();
  for (auto name : name_index) {
    name.Print();
  }
  
  const auto& id_index_hashed = es.get<2>();
  std::cout << "get a view to index #0 hashed (id)\n";
  for (auto it : id_index_hashed) {
    it.Print();
  }
  
  id_index_hashed.find(2)->Print();
  es.get<3>().find("Third")->Print();
  //std::cout << "get from find: \n";
  //es.get<2>().find(2)->Print();
}

int main() {
    employee_set es;

    es.insert(employee(1, "First"));
    es.insert(employee(2, "Second"));
    es.insert(employee(3, "Third"));
    es.insert(employee(4, "Fourth"));
    es.insert(employee(5, "Fifth"));
    es.insert(employee(6, "Sixth"));
    print_out(es);

    std::ofstream ofs("savefile.txt");
    if (ofs.fail()) return false;
    boost::archive::text_oarchive oa(ofs);
    oa << es;
}