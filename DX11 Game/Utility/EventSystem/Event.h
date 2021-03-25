#pragma once

//Holds the list of possible events
enum class EVENTID {
	Event1,
	Event2,
	Event3,
	Event4,
};

class Event {
public:

	//Constructor that sends event with data (cant be altered from the other side i believe)
	Event(EVENTID eventID, void* data) {
		this->data = data;
		this->eventID = eventID;
	}

	//Alternate constructor to just call an event proc without data
	Event(EVENTID eventID) {
		this->eventID = eventID;
		this->data = nullptr;
	}


	//Destructor
	~Event() { data = nullptr; }

	//Getters
	EVENTID GetEventID() const { return eventID; }
	void* GetData() { return data; }



private:

	EVENTID eventID;
	void* data;
};