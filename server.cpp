#include <rpc/rpc.h>
#include <string>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>
#include "database.h"

using namespace std;



struct databb{
	unsigned long int val_key;				//baza de date un utilizator
	vector<sensor_data> mydb;
};

vector< pair <char *,int> > users;			//vector useri

vector< databb > baza_date;					//vector baze de date

//adauga user
unsigned long int *add_1_svc(struct login *num, struct svc_req *cl) {
	static unsigned long int key;
	key = rand() % 1000000000000;
	for(int i=0;i<users.size();i++)
	{
		if(users[i].second == key)
			key = rand() % 1000000000000;

		if(strcmp(users[i].first, num->nume) == 0){
			key = 0;
		}

	}
	if(key!=0){
		char * userr = (char *)malloc(100);
		strcpy(userr,num->nume);
		users.push_back(make_pair(userr , key));
	}
	return &key;
}

//sterge user
login * dell_1_svc(struct struc_chei* key, struct svc_req *cl)
{
	for(int i=0;i<users.size();i++)
	{
		if(users[i].second == key->val)
			users.erase (users.begin()+i-1);
	}
	login *msgg;
	strcpy(msgg->nume,"userul a fost sters cu succes");
	return msgg;
}

//adauga data
message * adauga_1_svc(struct sensor_data* data_in, struct svc_req *cl)
{
	int adauga_db = 0;
	int adauga_data = 0;
	int index_gasit;
	int idx;
	message *msgg=new message;
	for(int i=0;i<baza_date.size();i++){
		if(baza_date[i].val_key == data_in->keyy){
			adauga_db = 1;
			index_gasit = i;
			for(int j=0;j< baza_date[i].mydb.size() ;j++){
				//printf("ba aici2");
				if(baza_date[i].mydb[j].data_id == data_in->data_id)
					adauga_data = 1;
					idx = j;
			}
		}
	}
			if(adauga_db == 0){
				databb *datab_new =new databb;
				datab_new->val_key = data_in->keyy;
				datab_new->mydb.push_back(*data_in);
				baza_date.push_back(*datab_new);
			}
			else if(adauga_data == 0){
				baza_date[index_gasit].mydb.push_back(*data_in);
			}
	if(adauga_data == 1){
		strcpy(msgg->mesg,"eroare adaugare (deja exista in baza de date)");
	}
	else{
		strcpy(msgg->mesg,"elemnt adaugat cu succes");
	}
	return msgg;
}

//sterge data
message * sterge_1_svc(struct key_id* data_in, struct svc_req *cl)
{
	int sterge_data = 0;
	int index_gasit;
	int idx;
	message *msgg=new message;
	for(int i=0;i<baza_date.size();i++){
		if(baza_date[i].val_key == data_in->key){
			index_gasit = i;
			for(int j=0;j< baza_date[i].mydb.size() ;j++){
				if(baza_date[i].mydb[j].data_id == data_in->id){
					sterge_data = 1;
					baza_date[i].mydb.erase (baza_date[i].mydb.begin()+j);
				}
			}
		}
	}
	if(sterge_data == 0){
		strcpy(msgg->mesg,"eroare stergere (nu se gaseste id)");
	}
	else{
		strcpy(msgg->mesg,"sters cu succes");
	}
	return msgg;
}

//update data
message * update_1_svc(struct sensor_data* data_in, struct svc_req *cl)
{
	int adauga_db = 0;
	int adauga_data = 0;
	int index_gasit;
	message *msgg=new message;
	for(int i=0;i<baza_date.size();i++){
		if(baza_date[i].val_key == data_in->keyy){
			adauga_db = 1;
			index_gasit = i;
			for(int j=0;j< baza_date[i].mydb.size() ;j++){
				if(baza_date[i].mydb[j].data_id == data_in->data_id){
					adauga_data = 1;
					baza_date[i].mydb[j]=*data_in;
				}
			}
		}
	}
			if(adauga_db == 0){
				printf("ba aici3%d",adauga_db);
				databb *datab_new =new databb;
				datab_new->val_key = data_in->keyy;
				datab_new->mydb.push_back(*data_in);
				baza_date.push_back(*datab_new);
			}
			else if(adauga_data == 0){
				printf("ba aici5%d",adauga_db);
				baza_date[index_gasit].mydb.push_back(*data_in);
			}
	if(adauga_data == 1){
		strcpy(msgg->mesg,"update facut cu succes");
	}
	else{
		strcpy(msgg->mesg,"adaugat cu succes");
	}
	return msgg;
}

//citeste inregistrare
message * read_1_svc(struct key_id* data_in, struct svc_req *cl)
{
	int adauga_db = 0;
	int adauga_data = 0;
	int index_gasit;
	string cit="";
	message *msgg=new message;
	for(int i=0;i<baza_date.size();i++){
		if(baza_date[i].val_key == data_in->key){
			adauga_db = 1;
			for(int j=0;j< baza_date[i].mydb.size() ;j++){
				if(baza_date[i].mydb[j].data_id == data_in->id){
					adauga_data = 1;
					cit = cit + to_string(baza_date[i].mydb[j].no_values)+" ";
					for(int k =0 ;k<baza_date[i].mydb[j].no_values;k++)
						cit = cit +to_string(baza_date[i].mydb[j].values[k])+" ";
					break;

				}
			}
		}
	}

	if(adauga_data == 0 || adauga_db == 0){
		strcpy(msgg->mesg,"nu s-a gasit id");
	}
	else{
		strcpy(msgg->mesg,cit.c_str());
	}
	return msgg;
}

//get_stat
gstat * getstat_1_svc(struct key_id* data_in, struct svc_req *cl)
{
	int adauga_db = 0;
	int adauga_data = 0;
	int index_gasit;
	string cit="";
	gstat *stat=new gstat;
	stat->gasit=0;
	int nr;
	for(int i=0;i<baza_date.size();i++){
		if(baza_date[i].val_key == data_in->key){
			adauga_db = 1;
			for(int j=0;j< baza_date[i].mydb.size() ;j++){
				if(baza_date[i].mydb[j].data_id == data_in->id){
					stat->gasit = 1;

					nr= baza_date[i].mydb[j].no_values;
					stat->min = baza_date[i].mydb[j].values[0];
					stat->max = baza_date[i].mydb[j].values[0];
					stat->med = baza_date[i].mydb[j].values[0];
					for(int k =1 ;k<baza_date[i].mydb[j].no_values;k++){
						stat->med += baza_date[i].mydb[j].values[k];
						if(stat->min > baza_date[i].mydb[j].values[k])
							stat->min = baza_date[i].mydb[j].values[k];
						if(stat->max < baza_date[i].mydb[j].values[k])
							stat->max = baza_date[i].mydb[j].values[k];
					}
					stat->med/=nr;
					sort(baza_date[i].mydb[j].values, baza_date[i].mydb[j].values +
						nr);
					if(baza_date[i].mydb[j].no_values %2 == 0 ){
						stat->par = 1;
						stat->median1 = baza_date[i].mydb[j].values[nr/2-1];
						stat->median2 = baza_date[i].mydb[j].values[(nr/2) ];
					}
					else
					{
						stat->par = 0;
						stat->median1 = baza_date[i].mydb[j].values[(nr/2)];
					}
					break;

				}
			}
		}
	}

	return stat;
}

//get_stat_all
gstat * getallstat_1_svc(struct struc_chei* data_in, struct svc_req *cl)
{
	int adauga_db = 0;
	int adauga_data = 0;
	int index_gasit;
	string cit="";
	float toate[10000];
	int iter=0;
	gstat *stat=new gstat;
	stat->gasit=0;
	int nr=0;
	stat->min = baza_date[0].mydb[0].values[0];
	stat->max = baza_date[0].mydb[0].values[0];
	stat->med = 0;
	for(int i=0;i<baza_date.size();i++){
		for(int j=0;j< baza_date[i].mydb.size() ;j++){
			nr += baza_date[i].mydb[j].no_values;
			for(int k =0 ;k<baza_date[i].mydb[j].no_values;k++){
				toate[iter] =  baza_date[i].mydb[j].values[k];
				iter++;
				stat->med += baza_date[i].mydb[j].values[k];
				if(stat->min > baza_date[i].mydb[j].values[k])
					stat->min = baza_date[i].mydb[j].values[k];
				if(stat->max < baza_date[i].mydb[j].values[k])
					stat->max = baza_date[i].mydb[j].values[k];
			}
		}
	}
	stat->med/=nr;
	sort(toate, toate + nr);
	if(nr %2 == 0 ){
		stat->par = 1;
		stat->median1 = toate[nr/2-1];
		stat->median2 = toate[(nr/2) ];
	}
	else
	{
		stat->par = 0;
		stat->median1 = toate[(nr/2)];
	}


	return stat;
}




