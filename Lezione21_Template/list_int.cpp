#include<string>
#include<iostream>

class List_int{
  public:
    List_int();
    List_int(int el);
    List_int(const List_int& source);

    void prepend(int el);    
    void append(int el);
    
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
  
  private:

    struct Cella{
      int info;
      Cella* next;
    };

    Cella* l;     //Punt. prima cella della lista  
    Cella* last;  //Punt. ultima cella della lista
    //Lista vuota codificata con l==last==nullptr
};

bool List_int::empty() const{

	return not l;

}

List_int& List_int::operator+=(List_int& rhs){

	//l1 += l1	
	if(this == &rhs){
		
		//we decide to empty l1
		while(not empty()) remove_first();
		return *this;
		
	}
	
	while(not rhs.empty()){
		
		append(rhs.first());
		rhs.remove_first();
		
	}
	
	return *this;
	
}


List_int& List_int::operator+=(const List_int& rhs){

	/*
	 *  L1 += L1 does not compile if l1 is const, however one could
	 *  call l1 += l1_c, where l1_c is a const reference to l1!
	 *  in this case we do not have a compilation error so we
	 *  need to consider the case this == &rhs also here
	 */
	 
	//in this case we decide to simply duplicate this
	if(this == &rhs){
		
		Cella* end = last;
		Cella* ptr = l;

		append(ptr->info);
					
		while(ptr!=end){
		
			ptr = ptr->next;	
			append(ptr->info);
			
		}
		
		return *this;
		
	}
	 
	Cella* ptr = rhs.l;
	
	while(ptr){
		
		append(ptr->info);
		ptr = ptr->next;
		
	}

	return *this;

}


bool List_int::operator==(const List_int& rhs) const{
	
	Cella* ptr1 = l;
	Cella* ptr2 = rhs.l;
	
	while(ptr1 and ptr2){
		
		if(ptr1->info != ptr2->info) return false;
		
		ptr1 = ptr1->next;
		ptr2 = ptr2->next;
		
	}

	return not(ptr1 or ptr2);

}

List_int::List_int() :l{nullptr}, last{nullptr}  {
} //Crea la lista vuota
    
List_int::List_int(int el) {     
  l = new Cella;
  l->info=el;
  l->next=nullptr;
  last=l;
}

List_int::List_int(const List_int& source) {

  l = nullptr;
  last = nullptr;
  Cella* pc = source.l;
 
  while (pc) {
    append(pc->info);
    pc = pc->next;
  }
}
    
void List_int::prepend(int el) {
  Cella* nuova = new Cella;
  nuova->info = el;
  nuova->next = l;
  l = nuova;
  if (last==nullptr)
    last=nuova;
}

void List_int::append(int el) {
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

std::string List_int::convert_to_string() const{
  std::string res{};
  Cella* pc{l};

  while (pc) {
    res = res + std::to_string(pc->info) + " ";
    pc = pc->next;
  }
    return res;
}


List_int::~List_int() {
  Cella* pc;
  while(l) {
    pc = l;
    l=l->next;
    delete pc;
  }
}


int& List_int::first() {  
  return l->info;
}


const int& List_int::first() const {
  return l->info;
}

void List_int::remove_first() {
  if (l) {
    Cella* pc = l;
    l=l->next;
    delete pc;
    if (l==nullptr) 
      last = nullptr; 
  }
}


void read_list(List_int& l) {
  int elem, member;
  std::cin>>elem;

  while (elem>0) {
    std::cin >> member;
    l.append(member);
    elem--;
  }
}


int main() {
	
	List_int l1;
	List_int l2_nc;
	
	read_list(l1);
	read_list(l2_nc);	
	
	const List_int& l2 = l2_nc;
	
	std::cout << "l1: " << l1.convert_to_string() << std::endl;
	std::cout << "l2: " << l2.convert_to_string() << std::endl;
	
	l1 += l2;
	
	std::cout << "l1: " << l1.convert_to_string() << std::endl;
	std::cout << "l2: " << l2.convert_to_string() << std::endl;	
	
}
