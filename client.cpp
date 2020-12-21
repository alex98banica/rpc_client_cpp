#include <string>
#include <rpc/rpc.h>
#include <iostream>
#include <vector>
#include <iomanip>

#define PROTOCOL "tcp"
#define RMACHINE "localhost"
#include "database.h"

using namespace std;

int main(int argc, char const *argv[])
{

	CLIENT *handle;

	handle = clnt_create(RMACHINE, DATA, DATABASE, PROTOCOL);

	if (!handle) {
		perror("Error client handle");
		return -1;
	}

    if (argc < 1) {
        cout << "Usage: " << argv[0] << " <file_open>" << endl;
        return EXIT_FAILURE;
    }
    FILE *fp;
	fp = fopen(argv[1] , "r");
	if(fp == NULL) {
    	perror("Error opening file");
    	return(-1);
   	}
    char line[100];
    handle = clnt_create(RMACHINE, DATA, DATABASE, PROTOCOL);
    unsigned long int *key;
    while(fgets(line,100,fp)){
    	login user;
    	string s;
    	s.assign(line,100) ;
   		vector<string> result;
    		istringstream iss(s);
    	for(string s; iss >> s; ){
        	result.push_back(s);
    	}
    	//login user
        if(result[0]=="login"){
        	const char *cstr = result[1].c_str();
    		strcpy(user.nume ,cstr);
			key = add_1(&user, handle);
			if(*key == 0)
				cout << "eroare exista deja user"<<"\n\n";
			else
				cout << "key = " << *key<<"\n\n";
		}

		//logout
		else if(result[0]=="logout"){

			struc_chei cheie;
			cheie.val=*key;
			login *msg;
			msg	=dell_1(&cheie,handle);
			cout<<msg->nume<<"\n\n";
		}

		//adauga element
		else if(result[0]=="add"){
			sensor_data dataa;
			int idd = stoi( result[1] );
			int nr = stoi( result[2] );
			dataa.data_id = idd;
			dataa.no_values = nr;
			dataa.keyy = *key;
			for(int i=0; i<nr;i++)
			{
				dataa.values[i]=stof( result[i+3] );
			}

			message *msg = new message;

			msg	=adauga_1(&dataa,handle);
			cout<<msg->mesg<<"\n\n";
		}

		//sterge element
		else if(result[0]=="del"){
			key_id dataa;
			int idd = stoi( result[1] );
			dataa.id = idd;
			dataa.key = *key;
			message *msg = new message;

			msg	=sterge_1(&dataa,handle);
			cout<<msg->mesg<<"\n\n";
		}

		//update element
		else if(result[0]=="update"){
			sensor_data dataa;
			int idd = stoi( result[1] );
			int nr = stoi( result[2] );
			dataa.data_id = idd;
			dataa.no_values = nr;
			dataa.keyy = *key;
			for(int i=0; i<nr;i++)
			{
				dataa.values[i]=stof( result[i+3] );
			}
			message *msg = new message;

			msg	=update_1(&dataa,handle);
			cout<<msg->mesg<<"\n\n";
		}

		//read elemnt cu id
		else if(result[0]=="read"){
			key_id dataa;
			int idd = stoi( result[1] );
			dataa.id = idd;
			dataa.key = *key;
			message *msg = new message;

			msg	=read_1(&dataa,handle);
			cout<<"READ: "<<msg->mesg<<"\n\n";
		}

		//get_stat
		else if(result[0]=="get_stat"){
			key_id dataa;
			int idd = stoi( result[1] );
			dataa.id = idd;
			dataa.key = *key;
			gstat *stat = new gstat;

			stat =getstat_1(&dataa,handle);
			if(stat->gasit == 0)
				cout<<"id-ul nu a fost gasit\n";
			else{
				cout<<"MIN: "<<stat->min<<"\n";
				cout<<"MAX: "<<stat->max<<"\n";
				cout<<"MEDIA: "<<stat->med<<"\n";
				if(stat->par == 1)
				{
					cout<<"MEDIANELE SUNT: "<<stat->median1<<" ";
					cout<<stat->median2<<"\n\n";
				}
				else{
					cout<<"MEDIANA E: "<<stat->median1<<"\n\n";
				}
			}
		}

		//get_stat_all
		else if(result[0]=="get_stat_all"){
			struc_chei dataa;
			dataa.val = *key;
			gstat *stat = new gstat;

			stat =getallstat_1(&dataa,handle);
			cout<<"MIN: "<<stat->min<<"\n";
			cout<<"MAX: "<<stat->max<<"\n";
			cout<<"MEDIA: "<<stat->med<<"\n";
			if(stat->par == 1){
				cout<<"MEDIANELE SUNT: "<<stat->median1<<" ";
				cout<<stat->median2<<"\n\n";
			}
			else{
				cout<<"MEDIANA E: "<<stat->median1<<"\n\n";
			}
		}
	}
	clnt_destroy(handle);

	return 0;
}