// Academic Project 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 4996) // for Visual Studio

#define MAX_NAME 30

// global linked list 'list' contains the list of patients
struct patientList 
{
	struct patient *patient;
	struct patientList *next;
} *list = NULL;				// currently empty list

// structure "patient" contains the patient's name, room number and linked list of doctors
struct patient {
	char name[MAX_NAME];
	unsigned int roomNumber;
	struct doctor *doctors;		// linked list 'doctors' contains names of doctors
};

//  structure 'doctor' contains doctor's name 
struct doctor {
	char name[MAX_NAME];
	struct doctor *next;
};

// forward declaration of functions (already implmented)
void flushStdIn();
void executeAction(char);

// functions that need implementation:
void addPatient(char* patientNameInput, unsigned int roomNumInput);
struct patient* searchPatient(char* patientNameInput);
void displayList(struct patientList* tempList);
void addDoctor(char* patientNameInput, char* doctorNameInput);
void removePatient(char* patientNameInput);	
char* lastDoctor(char* patientNameInput);
struct patientList* patientsOnThisFloor(unsigned int floorNumber);
void deleteList(struct patientList* pList);


int main()
{
	char selection = 'a';		// initialized to a dummy value
	do
	{
		printf("\nCSE220 Project 5\n");
		printf("Please enter your selection:\n");
		printf("\t a: add a new patient to the list\n");
		printf("\t d: display patient list\n");
		printf("\t c: add a doctor of a patient\n");		
		printf("\t r: remove a patient\n");
		printf("\t l: display last added doctor of a patient\n");
		printf("\t b: display patients on a floor\n");
		printf("\t q: quit\n");

		selection = getchar();
		flushStdIn();
		executeAction(selection);
	} while (selection != 'q');

	return 0;
}

// flush out leftover '\n' characters
void flushStdIn()
{
	char c;
	do c = getchar();
	while (c != '\n' && c != EOF);
}

// Ask for details from user for the given selection and perform that action
// Read the function case by case
void executeAction(char c)
{
	char patientNameInput[MAX_NAME], doctorNameInput[MAX_NAME];
	unsigned int roomNumInput, floorNumber;
	struct patient* searchResult = NULL;
	int n;

	switch (c)
	{
	case 'a':	// add patient
				// input patient details from user
		printf("\nPlease enter patient's name: ");
		fgets(patientNameInput, sizeof(patientNameInput), stdin);
		patientNameInput[strlen(patientNameInput) - 1] = '\0';	// discard the trailing '\n' char
		printf("Please enter room number: ");
		scanf("%d", &roomNumInput);
		flushStdIn();

		if (searchPatient(patientNameInput) == NULL)					
		{
			addPatient(patientNameInput, roomNumInput);
			printf("\nPatient successfully added to the list!\n");
		}
		else
			printf("\nThat patient is already on the list!\n");
		break;

	case 'd':		// display the list
		displayList(list);
		break;

	case 'c':		// add doctor
		printf("\nPlease enter patient's name: ");
		fgets(patientNameInput, sizeof(patientNameInput), stdin);
		patientNameInput[strlen(patientNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchPatient(patientNameInput) == NULL)					
			printf("\nPatient name does not exist or the list is empty! \n\n");
		else
		{
			printf("\nPlease enter doctor's name: ");
			fgets(doctorNameInput, sizeof(doctorNameInput), stdin);
			doctorNameInput[strlen(doctorNameInput) - 1] = '\0';	// discard the trailing '\n' char

			addDoctor(patientNameInput, doctorNameInput);
			printf("\nDoctor added! \n\n");
		}
		break;

	case 'r':		// remove patient
		printf("\nPlease enter patient's name: ");
		fgets(patientNameInput, sizeof(patientNameInput), stdin);
		patientNameInput[strlen(patientNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchPatient(patientNameInput) == NULL)				
			printf("\nPatient name does not exist or the list is empty! \n\n");
		else
		{
			removePatient(patientNameInput);
			printf("\nPatient successfully removed from the list! \n\n");
		}
		break;

	case 'l':		// last doctor
		printf("\nPlease enter patient's name: ");
		fgets(patientNameInput, sizeof(patientNameInput), stdin);
		patientNameInput[strlen(patientNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchPatient(patientNameInput) == NULL)				
			printf("\nPatient name does not exist or the list is empty! \n\n");
		else
		{
			printf("\nlast doctor added: %s\n\n", lastDoctor(patientNameInput));
		}
		break;

	case 'b':		// patients on a floor
		printf("Please enter floor number: ");
		scanf("%d", &floorNumber);
		flushStdIn();
		struct patientList* result = patientsOnThisFloor(floorNumber);
		displayList(result);
		deleteList(result);
		break;

	case 'q':		// quit
		deleteList(list);
		break;

	default: printf("%c is invalid input!\n", c);
	}
}

// Q1: addPatient (10 points)
// This function is used to insert a new patient in the linked list. You must insert the new patient to the head of linked list 'list'.
// You need NOT check if the patient already exists in the list because that is taken care by searchPatient() called in executeAction(). Look at how this function is used in executeAction().
// Don't bother to check how to implement searchPatient() while implementing this function. Simply assume that patient does not exist in the list while implementing this function.
// NOTE: This function does not add doctors to the patient info. There is another function addDoctor() for that.
// NOTE: Implement searchPatient() to test addPatient() function because of the way addPatient() is used in executeAction()
//		 Or you can temporarily comment out searchPatient() in executeAction() to test addPatient()
// Hint: In this question, no doctors means NULL doctors.

void addPatient(char* patientNameInput, unsigned int roomNumInput)
{
	struct patient* pat;					// Initializes a patient
	pat = malloc(sizeof(struct patient));	// Allocates memory for the patient 
	strcpy(pat->name, patientNameInput);	// Copies the name of the patient intialzied	
	pat->roomNumber = roomNumInput;			
	pat->doctors = NULL;					
	
	// This begins the list with a head node
	struct patientList* currentNode = list;	

	// This if statement checks to see if the head node is null
	if(currentNode == NULL)
	{
		list = malloc(sizeof(struct patientList));
		list->patient = pat;		// Adds the patient at null
		list->next = NULL;			// Sets next pointer to null
		return;
	}
	// This while loop keeps checking all the nodes in the list until it reaches the end of the list 
	while(currentNode->next != NULL)
	{
		currentNode = currentNode->next;
	}

	//This allocates memory for the new node (In this case the patient)
	struct patientList* NODE = malloc(sizeof(struct patientList));
	NODE->next = NULL; 
	NODE->patient = pat;		// Adds a patient (pat)
	currentNode->next = NODE;	// Moves currentNode to the last node
}
// Q2: searchPatient (10 points)
// This function searches the 'list' to check if the given patient exists in the list. Search by patient name.
// If it exists then return that 'patient' node of the list. Notice the return type of this function.
// If the patient does not exist in the list, then return NULL.

struct patient* searchPatient(char* patientNameInput)
{
	struct patientList* tempList = list;			// work on a copy of 'list'
	// Searches for the patient in the linked list as long as the list is not empty
	while(tempList != NULL)
	{
		struct patient* pat = tempList->patient;

		// Determines whether or not the patient being searched for is in the list
		if(strcasecmp(pat->name, patientNameInput) == 0) 
		{																	 
			return pat;
		}
		tempList = tempList->next;
	}
	return NULL;
}

// Q3: displayList (10 points)
// This function displays every detail(name, room number, doctors) of each patient.
// Parse through the linked list 'tempList' passed as parameter and print the patient details one after the other. 
// See expected output screenshots in homework question file.

void displayList(struct patientList* tempList)
{
	// If the list is empty, let the user know
	if(tempList == NULL)
	{
		printf("The list is empty\n");
	}
	struct patient* pat = tempList->patient;	// Catches the patient structure
	printf("%p\n%d\n%p\n", pat->name, pat->roomNumber, pat->doctors);
	displayList(tempList->next);		// Calls to the next node (recursively)
}

// Q4: addDoctor (15 points)
// This function adds doctor's name to a patient node.
// Parse the list to locate the patient and add the doctor to that patient's 'doctors' linked list. No need to check if the patient name exists on the list. That is done in executeAction().
// You can assume that the same doctor name does not exist. So no need to check for existing doctor names.
// You are free to add new doctor to head or tail of 'doctors' list. (Sample solution has doctor added to the tail of 'doctors' list.)

void addDoctor(char* patientNameInput, char* doctorNameInput)
{
	struct patientList* tempList = list;		// work on a copy of 'list'
	while(tempList != NULL)
	{
		struct patient* pat = tempList->patient;
		// If patient name is found, perform the following
		if(strcasecmp(pat->name, patientNameInput) == 0) 
		{
			//This creates a doctor for the patient
			struct doctor* dr = pat->doctors; 
			struct doctor* DOCTOR = malloc(sizeof(struct doctor));
			strcpy(DOCTOR->name, doctorNameInput);		// Copies the name of the doctor intialized
			DOCTOR->next = NULL;

			// If there is no doctor assigned to a patient, then peform the following
			if(dr == NULL)
			{
				pat->doctors = DOCTOR;	// Sets up the head node
				return;
			}
			while(dr->next != NULL)
			{
				dr = dr->next;
			}
			dr->next = DOCTOR;			// Adds at the tail node
			return;
		}
		tempList = tempList->next;
	}
}

// Q5: removePatient (15 points)
// This function removes a patient from the list.
// Parse the list to locate the patient and delete that 'patient' node.
// You need not check if the patient exists because that is done in executeAction()
// NOTE: You need to remove the doctors too, if any.

void removePatient(char* patientNameInput)
{
	struct patientList* tempList = list;	// work on a copy of 'list'
	struct patientList* previousNode;
	
	// If there's only one node in the list, then the "next" node is empty
	if(tempList->next == NULL)
	{
		list = NULL;
	}

	while(tempList != NULL)
	{
		struct patient* pat = tempList->patient;
		// If the name of the patient is found, peform the following
		if(strcasecmp(pat->name, patientNameInput) == 0)
		{
			// This skips the current node (tempList) by pointing to the previous nodes next to the current node's "next" node 
			previousNode->next = tempList->next;
			return;
		}
		previousNode = tempList;
		tempList = tempList->next;
	}
}

// Q6: lastDoctor (10 points)
// This function returns the name of the last (most recently) added doctor of a patient.
// Parse the list to locate the patient. No need to check if the patient name exists in the list. That is done in executeAction().
// Then parse the doctor names to return the most recently added doctor.
// NOTE: Last doctor does not necessarily mean the tail of 'doctors' list. It means the most recently added doctor. 
//       If you are adding doctor to the head of the list in addDoctor(), then you should return that doctor name accordingly.

char* lastDoctor(char* patientNameInput)
{
	
	struct patientList* tempList = list;		// work on a copy of 'list'
	// This begins with the head node and loops until it reaches the end (tail) of the list
	while(tempList != NULL)
	{
		struct patientList* pList = tempList;
		// If patient exists in the list, peform the following
		if(strcasecmp(pList->patient->name, patientNameInput))
		{
			struct doctor* dr = pList->patient->doctors;
			// If there are no doctors assigned to the patient, then return NULL letting
			// the user know that the patient doesn't have a doctor assigned
			if(dr = NULL)
			{
				return NULL;
			}
			//This loops until it reaches the tail of the list
			while(dr->next != NULL)
			{
				dr = dr->next;
			}
			return dr->name;
		}
		tempList = tempList->next;
	}
	return NULL;	
}


// Q7: patientsOnThisFloor (20 points)
// This function is used to construct a new linked list 'resultList' from the global linked list 'list'.
// The returned list should contain the patients whose floor number matches with the input parameter 'floorNumber'.
// Input the room number as a 3-digit number and first digit is the floor number. Eg-room 235 means floor 2.
// No sorting is required for this list.
// The list that you return will be printed by displayList() and then cleaned up by the deleteDatabase() function. This is done in case 'b' of executeAction().
// Note that the returned list will need to contain doctor information too.
// This function should NOT modify the global linked list 'list'.
struct patientList* patientsOnThisFloor(unsigned int floorNumber)
{
	struct patientList* tempList = list;	// work on a copy of 'list'
	struct patientList* resultList = NULL;
	// initializes another pointer to add the patients
	struct patientList* resultPTR = resultList;
	// The following while loop finds all the patients 
	// on the floor inputed by the user
	while(tempList != NULL)
	{
		if(tempList->patient->roomNumber / 100 == floorNumber)
		{
			// This copies the patients information (data) into the current list (tempList)
			struct patient* cpyData = (struct patient*)malloc(sizeof(struct patient));
			// The following copies the patient's data 
			// and assigns the patient to a specficed room number
			cpyData->roomNumber = tempList->patient->roomNumber;
			strcpy(cpyData->name, tempList->patient->name);
			// Sets the doctor's data to null because no data for the doctor has been created yet
			cpyData->doctors = NULL;
			
			// If the list of doctors in the current list (tempList) is not empty, then it copies the doctor's data into tempList
			if(tempList->patient->doctors != NULL)
			{
				cpyData->doctors = (struct doctor*)malloc(sizeof(struct doctor));
				strcpy(cpyData->doctors->name, tempList->patient->doctors->name);
				cpyData->doctors->next = NULL;
				// This intializes two pointers in order to copy the remaining doctor's data from one location to another
				struct doctor* cpyDataFrom = tempList->patient->doctors;
				struct doctor* cpyDataTo = cpyData->doctors;
				
				// This while loops copies each doctor's data in the list until there is no data left to copy
				while(cpyDataFrom->next != NULL) 
				{
					cpyDataTo->next = (struct doctor*)malloc(sizeof(struct doctor));
					strcpy(cpyDataTo->next->name, cpyDataFrom->next->name);
					// This moves to the next node
					cpyDataTo = cpyDataTo->next;
					cpyDataFrom = cpyDataFrom->next;
					cpyDataTo->next = NULL;
				}
			}
			// This if-else statement checks to see if the result list is empty
			// If the result list is not empty, then it modifies the pointer for the result list 
			if(resultList == NULL)
			{
				// This initializes the result list
				resultList = (struct patientList*)malloc(sizeof(struct patientList));
				// This adds the first patient from the specified floor
				resultList->patient = cpyData;
				resultList->next = NULL;
				// This resets the pointer for the result List
				resultPTR = resultList;
			}
				// This adds the patient to the result list
				resultPTR->patient = cpyData;
				// This moves the pointer for the result list to the "next" pointer
				resultPTR = resultPTR->next;
				// Sets the next pointer to be the tail of the list
				resultPTR->next = NULL;
		}
		tempList = tempList->next;
	}
	return resultList;	// result list contains the search result
}
// Q8: deleteList (10 points)
// Delete all nodes and linked lists inside the passed argument 'pList'
// This fucntion is used to delete the linked list before exiting the program.

void deleteList(struct patientList* pList)
{
	// If this list is empty return nothing (no output to the user)
	if (!list)
	{
		return;
	}
	// This deletes the next node in the list recursively
	deleteList(list->next);
	// Initializs the patient in the current node
	struct patientList* patientList = list;

	// If a patient exists in the list, then perfrom the following
	if(patientList)
	{
		// This deallocates the memory that was previously assigned to the name of the current patient
		free(patientList); 
		// This deletes the list of doctors recursively
		deleteDoctors(pList->patient->doctors); 
		// This deallocates the memory that was previously assigned in the current patient
		free(patientList->patient); 
	}
	// This deallocated the memory that was previosly assigned in the list
	free(list);
}
