#include "TrafficModel.h"

TrafficModel::TrafficModel() { }
TrafficModel::~TrafficModel(){ }

void TrafficModel::set_commands(vector<string> commands)
{
	this->commands = commands;
}

/* A helper function. 
 * Given a specific id as the input, it searches the lane change command stored in
 * "commands". Returns the value of the turn-light in the command for this car id.
 * 0 = no light, 1 = left light, 2 = right light.
 */
int TrafficModel::get_lane_change_command(int id)
{
	int count = commands.size();
	for (int i = 0; i < count; i++){
		size_t found = commands[i].find(',');
		string iter_id = commands[i].substr(0, found);
		string light = commands[i].substr(found+1, commands[i].size()-1);
		stringstream ss1(iter_id);
		int id_value;
		ss1 >> id_value;
		if (id_value == id){
			stringstream ss2(light);
			int light_value;
			ss2 >> light_value;
			return light_value;
		}
	}
	return 0;
}

/*
 * The function that updates the vehicle positions and states.
 */
void TrafficModel::update()
{
	// TODO: complete this function

	int lane_change_command = 0;
	 int size = platoons.size();// the size of platoons
	 int lengths[4] = {0};//the length of platoons[i]
	 for(int k = 0 ; k < 4; k++){
		 lengths[k] = platoons[k].get_length();
	 }
	 for (int i = 0; i < size; i++){
	 	
	 	Car *item = platoons[i].get_head();//since from head
	 	Car *ta = NULL;
	 	
	 	int j = 0;
	 	while(j < lengths[i]){//from the head to tail
 	 		lane_change_command = get_lane_change_command(item->get_id());//get the lane change command
	 	
	 		if(0 == lane_change_command){
	 			item->set_position(item->get_position() + 1);
	 		}
	 		if(1 == lane_change_command){//turn left
	 			if(0 == i){
	 				item->set_position(item->get_position() + 1);
	 			}else{
	 				ta = new Car(*item);//new another place
	 				platoons[i - 1].insert(ta);//insert into left
	 				platoons[i].remove(item);//then, delete the item in this list
	 				item = platoons[i].get_Car(j);//the item is NULL, so we need to find the primary node
	 			}
	 		}
	 		if(2 == lane_change_command){//turn right
	 			if((size - 1) == i){
	 				item->set_position(item->get_position() + 1);
	 			}else{
	 				
	 				ta = new Car(*item);
	 				platoons[i + 1].insert(ta);
	 				platoons[i].remove(item);
	 				item = platoons[i].get_Car(j);
	 			}
	 		}
	 		
	 		if(item == platoons[i].get_tail() || item == NULL)
	 			break;
	 		item = item->get_next();	
	 		j++;
	 	}
	 //	platoons[i].All_car();
	 //	cout <<endl;
	 }
}


/*
 * Initialization based on the input information
 */
void TrafficModel::initialize(vector<string> info)
{
	int lane_count = info.size();
	for (int i = 0; i < lane_count; i++){
		Platoon p = Platoon(info[i]);
		platoons.push_back(p);
	}
}

// Returns all the vehicle states in the system
vector<string> TrafficModel::get_system_state()
{
	vector<string> output;
	int size = platoons.size();
	for (int i = 0; i < size; i++){
		// get the last vehicle in the platoon
		Car* temp = platoons[i].get_tail();
		string s = "";
		ostringstream out;
		while (temp != NULL){
			out << ";(" << temp->get_id() << "," << i << "," << temp->get_position() << \
					 "," << get_lane_change_command(temp->get_id()) << ")";
			temp = temp->get_prev();
		}

		output.push_back(out.str());
	}
	return output;
}
