#include<iostream>
#define bool int
#define true 1
#define false 0
using namespace std;
class Iterator
{
private:
Iterator *iterator;
public:
Iterator()
{
this->iterator=NULL;
}
Iterator(Iterator *iterator)
{
this->iterator=iterator;
}
Iterator(const Iterator &other)
{
this->iterator=other.iterator;
}
Iterator & operator=(const Iterator &other)
{
this->iterator=other.iterator;
return *this;
}
virtual int hasMoreElements()
{
if(iterator!=NULL) return this->iterator->hasMoreElements();
return 0;
}
virtual int next()
{
if(iterator!=NULL) return this->iterator->next();
return 0;
}
};
class Iterable
{
public:
virtual Iterator getIterator()=0;
};
class TMList
{
public:
virtual void add(int data, bool *success)=0;
virtual int get(int index, bool *success) const=0;
virtual int getSize() const=0 ;
virtual void update(int index, int data, bool *success)=0;
virtual void insertAt(int index, int data, bool *success)=0;
virtual int removeAt(int index, bool *success)=0;
virtual void removeAll()=0;
virtual void clear()=0;
};

/*
	TMList cateogry classes should also contain
	  1. Default Constructor
	  2. Parameterized Constructor
	  3. Copy Constructor
	  4. Destructor
	  5. Operator =
	  6. Operator +=
	  7. Operator +
*/

class TMArrayList: public TMList
{
private:
int ** ptr;
int capacity;
int size;
char allocationFlag;
bool addRow();

public:
TMArrayList();
TMArrayList(int bufferSize);
TMArrayList(const TMArrayList &other);
virtual ~TMArrayList();
TMArrayList & operator=(const TMArrayList &other);
void operator+=(const TMArrayList &other);
TMArrayList operator+(const TMArrayList &other);

TMArrayList(const TMList &other);
TMArrayList & operator=(const TMList &other);
void operator+=(const TMList &other);
TMArrayList operator+(const TMList &other);


void add(int data, bool *success);
int get(int index, bool *success) const;
int getSize() const;
void update(int index, int data, bool *success);
void insertAt(int index, int data, bool *success);
int removeAt(int index, bool *success);
void removeAll();
void clear();
};

bool TMArrayList::addRow()
{
if(this->capacity%100==0)
{
int **tmp=new int *[(this->capacity/10)+10];
if(tmp==NULL) return false;
for(int e=0; e<(this->capacity/10); e++) tmp[e]=this->ptr[e];
delete [] this->ptr;
this->ptr=tmp;
}

this->ptr[this->capacity/10]=new int [10];
if(this->ptr[this->capacity/10]==NULL) return false;
this->capacity=this->capacity+10;
return true;
}
TMArrayList::TMArrayList()
{
this->ptr=new int *[10];
this->ptr[0]=new int [10];
this->size=0;
this->capacity=10;
this->allocationFlag=0;
}
TMArrayList::TMArrayList(int bufferSize)
{
if(bufferSize<=0)
{
this->ptr=new int *[10];
this->ptr[0]=new int [10];
this->size=0;
this->capacity=10;
this->allocationFlag=0;
}
else
{
int rows, numberOfPointers;
rows=bufferSize/10;
if(bufferSize%10!=0) rows++;
numberOfPointers=rows+(10-(rows%10));
this->ptr= new int * [numberOfPointers];
for(int e=0; e<rows; e++) this->ptr[e]=new int [10];
this->size=0;
this->capacity=rows*10;
}
}
TMArrayList::TMArrayList(const TMArrayList &other)
{
int bufferSize=other.size;
if(bufferSize<=0)
{
this->ptr=new int *[10];
this->ptr[0]=new int [10];
this->size=0;
this->capacity=10;
this->allocationFlag=0;
}
else
{
int rows, numberOfPointers;
rows=bufferSize/10;
if(bufferSize%10!=0) rows++;
numberOfPointers=rows+(10-(rows%10));
this->ptr= new int * [numberOfPointers];
for(int e=0; e<rows; e++) this->ptr[e]=new int [10];
this->size=0;
this->capacity=rows*10;
}
bool succ;
for(int e=0; e<other.size; e++) this->add(other.get(e, &succ), &succ);
}
TMArrayList::~TMArrayList()
{
if(allocationFlag==0)
{
for(int e=0; e<this->capacity/10; e++) delete [] this->ptr[e];
delete [] this->ptr;
}
}
TMArrayList & TMArrayList::operator=(const TMArrayList &other)
{
if(other.allocationFlag==1)
{
this->ptr=other.ptr;
this->size=other.size;
this->capacity=other.capacity;
}
else
{
this->size=0;
bool succ;
for(int e=0; e<other.size; e++) this->add(other.get(e,&succ), &succ);
}
return *this;
}
void TMArrayList::operator+=(const TMArrayList &other)
{
bool succ;
for(int e=0; e<other.size; e++) this->add(other.get(e,&succ), &succ);
}
TMArrayList TMArrayList::operator+(const TMArrayList &other)
{
TMArrayList k;
bool succ;
for(int e=0; e<this->size; e++) k.add(this->get(e, &succ), &succ);
for(int e=0; e<other.size; e++) k.add(other.get(e, &succ), &succ);
k.allocationFlag=1;
return k;
}
void TMArrayList::add(int data, bool *success)
{
if(success) *success=false;
if(this->size==this->capacity)
{
if(!addRow()) return;
}
int rowIndex, columnIndex;
rowIndex=this->size/10;
columnIndex=this->size%10;
this->ptr[rowIndex][columnIndex]=data;
this->size++;
if(success) *success=true;
}
int TMArrayList::get(int index, bool *success) const
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
int rowIndex, columnIndex;
rowIndex=index/10;
columnIndex=index%10;
if(success) *success=true;
return this->ptr[rowIndex][columnIndex];
}
int TMArrayList::getSize() const
{
return this->size;
}
void TMArrayList::update(int index, int data, bool *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return;
int rowIndex, columnIndex;
rowIndex=index/10;
columnIndex=index%10;
this->ptr[rowIndex][columnIndex]=data;
if(success) *success=true;
}
void TMArrayList::insertAt(int index, int data, bool *success)
{
if(success) *success=false;
if(index<0 || index>this->size) return;
bool succ;
if(index==this->size)
{
this->add(data, &succ);
return;
}
this->add(this->get(this->size-1, &succ), &succ);
if(!succ) return;
for(int e=this->size-3; e>=index;e--) this->update(e+1, this->get(e, &succ), &succ);
this->update(index, data, &succ);
if(success) *success=true;
}
int TMArrayList::removeAt(int index, bool *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
bool succ;
int data=this->get(index, &succ);
for(int j=index; j<=this->size-2; j++) this->update(j, this->get(j+1, &succ), &succ);
this->size--;
if(success) *success=true;
return data;
}
void TMArrayList::removeAll()
{
this->size=0;
}
void TMArrayList::clear()
{
this->size=0;
}

TMArrayList::TMArrayList(const TMList &other)
{
this->ptr=new int *[10];
this->ptr[0]=new int [10];
this->size=0;
this->capacity=10;
this->allocationFlag=0;
bool succ;
for(int e=0; other.getSize(); e++) this->add(other.get(e, &succ), &succ);
}
TMArrayList & TMArrayList::operator=(const TMList &other)
{
this->size=0;
bool succ;
for(int e=0; other.getSize(); e++) this->add(other.get(e, &succ), &succ);
return *this;
}
void TMArrayList::operator+=(const TMList &other)
{
bool succ;
for(int e=0; other.getSize(); e++) this->add(other.get(e, &succ), &succ);
}

TMArrayList TMArrayList::operator+(const TMList &other)
{
TMArrayList k;
bool succ;
for(int e=0; this->size; e++) k.add(this->get(e, &succ), &succ);
for(int e=0; other.getSize(); e++) k.add(other.get(e, &succ), &succ);
return k;
}

class TMForwardList: public TMList, public Iterable
{
class TMNode
{
public:
int data;
TMNode *next;
TMNode()
{
this->next=NULL;
}
};
private:
TMNode *start, *end;
int size;
char allocationFlag;
public:
class TMForwardListIterator: public Iterator
{
TMNode *ptr;
public:
TMForwardListIterator()
{
this->ptr=NULL;
}
void init(TMNode *ptr)
{
this->ptr=ptr;
}
TMForwardListIterator(const TMForwardListIterator &other)
{
this->ptr=other.ptr;
}
TMForwardListIterator & operator=(const TMForwardListIterator &other)
{
this->ptr=other.ptr;
return *this;
}
int hasMoreElements()
{
return this->ptr!=NULL;
}
int next()
{
if(this->ptr==NULL) return 0;
int data=this->ptr->data;
this->ptr=this->ptr->next;
return data;
}
};
private:
TMForwardListIterator tmForwardListIterator;
public:
Iterator getIterator()
{
tmForwardListIterator.init(this->start);
return Iterator(&tmForwardListIterator);
}
TMForwardList();
TMForwardList(int bufferSize);
TMForwardList(const TMForwardList &other);
virtual ~TMForwardList();
TMForwardList & operator=(const TMForwardList &other);
void operator+=(const TMForwardList &other);
TMForwardList operator+(const TMForwardList &other);

TMForwardList(const TMList &other);
TMForwardList & operator=(const TMList &other);
void operator+=(const TMList &other);
TMForwardList operator+(const TMList &other);

void add(int data, bool *success);
int get(int index, bool *success) const;
int getSize() const;
void update(int index, int data, bool *success);
void insertAt(int index, int data, bool *success);
int removeAt(int index, bool *success);
void removeAll();
void clear();
};
TMForwardList::TMForwardList()
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
}
TMForwardList::TMForwardList(int bufferSize)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
}
TMForwardList::TMForwardList(const TMForwardList &other)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
bool succ;  
(*this)=(other);
}
TMForwardList::~TMForwardList()
{
if(allocationFlag==0)this->clear();
}
TMForwardList & TMForwardList::operator=(const TMForwardList &other)
{
if(allocationFlag==1)
{
this->start=other.start;
this->end=other.end;
this->size=other.size;
}
else
{
this->size=0;
(*this)+=(other);
}
return *this;
}
void TMForwardList::operator+=(const TMForwardList &other)
{
bool succ;
for(int e=0; e<other.getSize(); e++) this->add(other.get(e, &succ), &succ);
}
TMForwardList TMForwardList::operator+(const TMForwardList &other)
{
TMForwardList k;
k+=(*this);
k+=(other);
k.allocationFlag==1;
return k;
}
void TMForwardList::add(int data, bool *success)
{
if(success) *success=false;
TMNode *t;
t=new TMNode;
t->data=data;
if(t==NULL) return;
if(this->start==NULL)
{
this->start=this->end=t;
}
else
{
this->end->next=t;
this->end=t;
}
this->size++;
if(success) *success=true;
}
int TMForwardList::get(int index, bool *success) const
{
if(success) *success=false;
if(index< 0 || index>=this->size) return 0;
TMNode *t;
int x;
for(x=0, t=this->start; x<index; x++, t=t->next);
if(success) *success=true;
return t->data;
}
int TMForwardList::getSize() const
{
return this->size;
}
void TMForwardList::update(int index, int data, bool *success)
{
if(success) *success=false;
if(index< 0 || index>=this->size) return;
TMNode *t;
int x;
for(x=0, t=this->start; x<index; x++, t=t->next);
t->data=data;
if(success) *success=true;
}
void TMForwardList::insertAt(int index, int data, bool *success)
{
if(success) *success=false;
if(index< 0 || index>this->size) return;
TMNode *newNode;
newNode= new TMNode;
newNode->data=data;
bool succ;
if(index==this->size)
{
this->add(data, &succ);
return;
}
TMNode *t, *j;
int x;
for(x=0, t=this->start; x<index; x++)
{
j=t;
t=t->next;
}
if(t==this->start)
{
newNode->next=this->start;
this->start=newNode;
}
else
{
newNode->next=t;
j->next=newNode;
}
this->size++;
if(success) *success=true;
}

int TMForwardList::removeAt(int index, bool *success)
{
int data=0;
if(success) *success=false;
if(index< 0 || index>this->size) data;
TMNode *t, *j;
int x;
for(x=0, t=this->start; x<index; x++)
{
j=t;
t=t->next;
}
data=t->data;
if(this->end==this->start)
{
this->start=this->end=NULL;
}
else  if(t==this->start) 
{
this->start=this->start->next;
}
else if(t==this->end)
{
j->next=NULL;
this->end=j;
}
else
{
j->next=t->next;
}
delete t;
this->size--;
if(success) *success=true;
return data;
}

void TMForwardList::removeAll()
{
this->clear();
}

void TMForwardList::clear()
{
TMNode *t;
t=this->start;
while(this->start)
{
t=this->start;
this->start=this->start->next;
delete t;
}
this->end=NULL;
this->size=0;
}


TMForwardList::TMForwardList(const TMList &other)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
bool succ;
for(int e=0; e<other.getSize(); e++) this->add(other.get(e, &succ), &succ);
}

TMForwardList & TMForwardList::operator=(const TMList &other)
{
this->clear();
bool succ;
for(int e=0; e<other.getSize(); e++) this->add(other.get(e, &succ), &succ);
return *this;
}

void TMForwardList::operator+=(const TMList &other)
{
bool succ;
for(int e=0; e<other.getSize(); e++) this->add(other.get(e, &succ), &succ);
}

TMForwardList TMForwardList::operator+(const TMList &other)
{
TMForwardList k;
k+=(*this);
k+=(other);
return k;
}

int mainIterator()
{
TMForwardList list1;
bool k;
for(int e=11; e<=20; e++) list1.add(e, &k);
Iterator iterator= list1.getIterator();
while(iterator.hasMoreElements()) cout<<iterator.next()<<" ";
cout<<endl;
return 0;
}

int main()
{
cout<<"Checking For TMForwardList: "<<endl;
TMForwardList list1;
bool k;
for(int e=201; e<=400; e++) list1.add(e, &k);
cout<<"List1 Size: "<<list1.getSize()<<endl;
for(int e=0; e<list1.getSize(); e++) cout<<list1.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------Operator =Checking -----------------------------------------------"<<endl;
TMForwardList list2;
list2=list1;
cout<<"List2 Size: "<<list2.getSize()<<endl;
for(int e=0; e<list2.getSize(); e++) cout<<list2.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------Operator +=Checking -----------------------------------------------"<<endl;
list2+=list1;
cout<<"List2 Size: "<<list2.getSize()<<endl;
for(int e=0; e<list2.getSize(); e++) cout<<list2.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------Operator + Checking -----------------------------------------------"<<endl;
TMForwardList list3;
list3=list2+list1;
cout<<"List3 Size: "<<list3.getSize()<<endl;
for(int e=0; e<list3.getSize(); e++) cout<<list3.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------Update Checking -----------------------------------------------"<<endl;
list3.update(0, 222222, &k);
list3.update(10,55555, &k);
list3.update(599, 99999, &k);
cout<<"List3 Size: "<<list3.getSize()<<endl;
for(int e=0; e<list3.getSize(); e++) cout<<list3.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------InsertAt Checking-----------------------------------------------"<<endl;
list2.insertAt(0, 11111, &k);
list2.insertAt(10,11111, &k);
list2.insertAt(20,11111, &k);
list2.insertAt(30,11111, &k);
list2.insertAt(40,11111, &k);
list2.insertAt(405,11111, &k);
cout<<"List2 Size: "<<list2.getSize()<<endl;
for(int e=0; e<list2.getSize(); e++) cout<<list2.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------RemoveAt Checking-----------------------------------------------"<<endl;
cout<<list2.removeAt(0, &k)<<endl;
cout<<list2.removeAt(27, &k)<<endl;
cout<<list2.removeAt(76, &k)<<endl;
cout<<list2.removeAt(list2.getSize()-1, &k)<<endl;
cout<<"List2 Size: "<<list2.getSize()<<endl;
for(int e=0; e<list2.getSize(); e++) cout<<list2.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------Copy Constructor Checking-----------------------------------------------"<<endl;
TMForwardList list4(list1);
cout<<"List4 Size: "<<list4.getSize()<<endl;
for(int e=0; e<list4.getSize(); e++) cout<<list4.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------RemoveAll Checking-----------------------------------------------"<<endl;
cout<<"List1 Size: "<<list1.getSize()<<endl;
cout<<"List2 Size: "<<list2.getSize()<<endl;
cout<<"List3 Size: "<<list3.getSize()<<endl;
cout<<"List4 Size: "<<list4.getSize()<<endl;
list1.removeAll();
list2.removeAll();
list3.removeAll();
list4.removeAll();
cout<<endl;
cout<<"List1 Size: "<<list1.getSize()<<endl;
cout<<"List2 Size: "<<list2.getSize()<<endl;
cout<<"List3 Size: "<<list3.getSize()<<endl;
cout<<"List4 Size: "<<list4.getSize()<<endl;
cout<<endl<<"*******************************************************************************************"<<endl;
return 0;
}

int mainR()
{
TMArrayList list1;
bool k;
for(int e=201; e<=400; e++) list1.add(e, &k);
cout<<"List1 Size: "<<list1.getSize()<<endl;
for(int e=0; e<list1.getSize(); e++) cout<<list1.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------Operator =Checking -----------------------------------------------"<<endl;
TMArrayList list2;
list2=list1;
cout<<"List2 Size: "<<list2.getSize()<<endl;
for(int e=0; e<list2.getSize(); e++) cout<<list2.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------Operator +=Checking -----------------------------------------------"<<endl;
list2+=list1;
cout<<"List2 Size: "<<list2.getSize()<<endl;
for(int e=0; e<list2.getSize(); e++) cout<<list2.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------Operator + Checking -----------------------------------------------"<<endl;
TMArrayList list3;
list3=list2+list1;
cout<<"List3 Size: "<<list3.getSize()<<endl;
for(int e=0; e<list3.getSize(); e++) cout<<list3.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------Update Checking -----------------------------------------------"<<endl;
list3.update(0, 222222, &k);
list3.update(10,55555, &k);
list3.update(599, 99999, &k);
cout<<"List3 Size: "<<list3.getSize()<<endl;
for(int e=0; e<list3.getSize(); e++) cout<<list3.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------InsertAt Checking-----------------------------------------------"<<endl;
list2.insertAt(0, 11111, &k);
list2.insertAt(10,11111, &k);
list2.insertAt(20,11111, &k);
list2.insertAt(30,11111, &k);
list2.insertAt(40,11111, &k);
list2.insertAt(405,11111, &k);
cout<<"List2 Size: "<<list2.getSize()<<endl;
for(int e=0; e<list2.getSize(); e++) cout<<list2.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------RemoveAt Checking-----------------------------------------------"<<endl;
cout<<list2.removeAt(0, &k)<<endl;
cout<<list2.removeAt(27, &k)<<endl;
cout<<list2.removeAt(76, &k)<<endl;
cout<<list2.removeAt(list2.getSize()-1, &k)<<endl;
cout<<"List2 Size: "<<list2.getSize()<<endl;
for(int e=0; e<list2.getSize(); e++) cout<<list2.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------Copy Constructor Checking-----------------------------------------------"<<endl;
TMArrayList list4(list1);
cout<<"List4 Size: "<<list4.getSize()<<endl;
for(int e=0; e<list4.getSize(); e++) cout<<list4.get(e, &k)<<" ";
cout<<endl<<"*******************************************************************************************"<<endl;
cout<<"---------------------RemoveAll Checking-----------------------------------------------"<<endl;
cout<<"List1 Size: "<<list1.getSize()<<endl;
cout<<"List2 Size: "<<list2.getSize()<<endl;
cout<<"List3 Size: "<<list3.getSize()<<endl;
cout<<"List4 Size: "<<list4.getSize()<<endl;
list1.removeAll();
list2.removeAll();
list3.removeAll();
list4.removeAll();
cout<<endl;
cout<<"List1 Size: "<<list1.getSize()<<endl;
cout<<"List2 Size: "<<list2.getSize()<<endl;
cout<<"List3 Size: "<<list3.getSize()<<endl;
cout<<"List4 Size: "<<list4.getSize()<<endl;
cout<<endl<<"*******************************************************************************************"<<endl;

return 0;
}