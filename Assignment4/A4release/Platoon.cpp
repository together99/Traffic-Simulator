#include "Platoon.h"

Platoon::Platoon(string init)
{
    // the string 'init' looks like:
    // a,b ; c,d ; ... where, a and c are the vehicle id, b and d are positions.
    stringstream ss(init);
	string segment;
	while(getline(ss, segment, ';'))
	{
		size_t found = segment.find(",");
		if (found!=std::string::npos){
			string id = segment.substr(0, found);
			string pos = segment.substr(found+1, segment.size()-1);

			// conversion to integer
			stringstream s1(id);
			int id_value;
			s1 >> id_value;
			stringstream s2(pos);
			int pos_value;
			s2 >> pos_value;
			Car* car = new Car(id_value, pos_value);

            if (head == NULL)
            {
                head = car;
                tail = car;
            }
            else
            {
                tail->set_next(car);
                car->set_prev(tail);
                tail = car;
                car->set_next(NULL);
            }
		}
	}
}

Car* Platoon::get_tail()
{
	return tail;
}

Car* Platoon::get_head()
{
	return head;
}


//insert a car from the tail
void Platoon::insert(Car *car)
{
	
	if (head == NULL)
        {
		head = car;
        	tail = car;
        	car->set_next(NULL);
        	car->set_prev(NULL);   
    	}
    	else
    	{
        	tail->set_next(car);
        	car->set_prev(tail);
        	tail = car;
       		car->set_next(NULL);
    	}
}

//remove a car from a list
void Platoon::remove(Car *car)
{ 
	Car *item=head;
	Car *ta=item;
	while(item->get_id() != car->get_id()){
		ta = item;
		item = item->get_next();
	}
	 
	if(item == head){
		if(head->get_next() == NULL){
			head=tail=NULL;
		}else{
			head=head->get_next();
		}
		free(item);
		return ;	
	}
		
	if(item != tail){
		
	
		ta->set_next(item->get_next());
		item->get_next()->set_prev(ta);
		free(item);
	}
	else{
		tail=ta;
		ta->set_next(NULL);
		free(item);
	}
	

		
}

//get the length of a list
int Platoon::get_length()
{
	int length=0;
	if(head == NULL)
		return 0;
	Car *item = head;
	while(item != tail){
		item = item->get_next();
		length++;
	}
	length++;
	return length;
}

//find the Nth node in a list
Car* Platoon::get_Car(int n)
{
	Car *car=head;
	if(0 == n)
		return head;
	n--;
	for(int i=0;i<n;i++){
		car = car->get_next();
	}
	return car;
}

