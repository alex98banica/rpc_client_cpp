
struct sensor_data{
	unsigned long int keyy;
	int data_id;
	int no_values;
	float values[100];
};



struct login{
	char nume[100];
};

struct struc_chei{
	unsigned long int val;
};

struct message{
	char mesg[100];
};

struct key_id{
	unsigned long int key;
	int id;
};

struct gstat
{
	float min;
	float max;
	float med;
	float median1;
	float median2;
	int gasit;
	int par;
};



program DATA{
	version DATABASE {
	unsigned long int ADD(struct login) = 1;
	login DELL(struct struc_chei) = 2;
	message ADAUGA(struct sensor_data) = 3;
	message UPDATE(struct sensor_data) = 4;
	message STERGE(struct key_id) = 8;
	message READ(struct key_id) = 5;
	gstat GETSTAT(struct key_id) = 6;
	gstat GETALLSTAT(struct struc_chei) = 7;

	} = 1;
} = 200000001;

