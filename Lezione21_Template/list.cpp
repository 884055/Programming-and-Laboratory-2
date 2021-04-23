#include<string>
#include<iostream>

using namespace std;

template<typename T>
class List {
  public:
    List() {
        l = nullptr;
        last = nullptr;
    }
    List(T el) {
        l = new Cella;
        l->info=el;
        l->next=nullptr;
        last=l;
    }

    List(const List<T>& source) {
        l = nullptr;
        last = nullptr;
        Cella* pc = source.l;
        
        while (pc) {
            append(pc->info);
            pc = pc->next;
        }
    }

    /*
    void prepend(T el);    
    void append(T el);
    std::string convert_to_string() const;
    int& first();
    const int& first() const;
    	
    void remove_first();

	bool empty() const;

    ~List_int();
    
    //does not modify arguments
    //returns boolean
    bool operator==(const List_int& rhs) const;
    
    // l1 = l2
    // overwrites the content of l1 with the content of l2
    // does not modify l2
    List_int& operator=(const List_int& rhs);

	// l3 = l1 + l2
	// inserts into l3 the concatenation of l1 and l2, 
	// does not modify l1 and l2
    List_int operator+(const List_int& rhs) const;

	// l1 += l2
	// appends l2 at the end of l1, empties l2
	List_int& operator+=(List_int& rhs);

	// l1 += l2
	// appends l2 at the end of l1, does not modify l2
	List_int& operator+=(const List_int& rhs);
  
  	// returns a reference to the info of the i-th cell
  	// l1[4] = 9
  	int& operator[](int i) const;
  */
  private:

    struct Cella{
      T info;
      Cella* next;
    };

    Cella* l;     //Punt. prima cella della lista  
    Cella* last;  //Punt. ultima cella della lista
    //Lista vuota codificata con l==last==nullptr


    void append(T el) {
        Cella* nuova = new Cella;
        nuova->info = el;
        nuova->next=nullptr;

        if (l==nullptr) {
            l=nuova;
            last=nuova;
        }
        else {
            last->next = nuova;
            last = nuova;
        }
    }
};


int main() {
    List<int> l_int = List<int>{1};
    List<double> l_double = List<double>{1.0};
    List<int> l1 = List<int>{l_int};
    return 0;
}