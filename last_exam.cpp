#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

class person // normal person
{
public:
    person() {}
    void setdata1(long num, int age, string gender);
    long getID() const
    {
        return ID;
    }
    int getage() const
    {
        return age;
    }
    string getgender() const
    {
        return gender;
    }

private:
    long ID;
    int age;
    string gender;
};

class police : public person
{
public:
    police() {}
    void setdata2(long num1, int num2, int ages, string genders)
    {
        setdata1(num1, ages, genders);
        police_ID = num2;
    }
    int getpoliceID() const
    {
        return police_ID;
    }

private:
    int police_ID;
};

class station // Information of nucleic acid collection station
{
public:
    int totalnum;       // total number of people that come to the station
    int num_minute[30]; // number of people that come to the station in each minute
    int personnum[30];  // number of people that come to the station in each minute
    int policenum[30];  // number of police that come to the station in each minute
    station();
    void setqueque_police(police **polices);
    void printqueue_police(int minute, int &current_num, int &total);
    void setqueque_person(person **persons, int minute);
    void printqueue_person(int minute);
    void print(police **polices, person **persons);

private:
    int stationnum; // number of normal nucleic acid collection station
    police police_queue[1500];
    person person_queue[135][10];
    int tag[135]; // Mark the current queue number of each common nucleic acid detection point
};

void person ::setdata1(long num, int ages, string genders)
{
    ID = num;
    age = ages;
    gender = genders;
}

station ::station()
{
    totalnum = 0;
    srand((unsigned)time(NULL));
    for (int i = 0; i < 30; i++)
    {
        num_minute[i] = 10 + rand() % 41;
        totalnum += num_minute[i];
        personnum[i] = rand() % (num_minute[i] + 1);
        policenum[i] = num_minute[i] - personnum[i];
    }
    stationnum = 1;
    for (int i = 0; i < 135; i++)
    {
        tag[i] = 0;
    }
}

void station ::setqueque_police(police **polices)
{
    int k = 0;
    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < policenum[i]; j++)
        {
            police_queue[k] = polices[i][j];
            k++;
        }
    }
}

void station ::printqueue_police(int minute, int &current_num, int &total)
{
    cout << "Current information of special collection points for police:" << endl;
    total += policenum[minute];
    if (total - current_num >= 5)
    {
        for (int i = current_num; i < total; i++)
        {
            cout << i - current_num + 1 << ". ID: " << police_queue[i].getID() << ",  age: " << police_queue[i].getage()
                 << ", gender: " << police_queue[i].getgender() << ", police ID: " << police_queue[i].getpoliceID() << endl;
        }
        current_num += 5;
    }
    else
    {
        for (int i = current_num; i < total; i++)
        {
            cout << i - current_num + 1 << ". ID: " << police_queue[i].getID() << ",  age: " << police_queue[i].getage()
                 << ", gender: " << police_queue[i].getgender() << ", police ID: " << police_queue[i].getpoliceID() << endl;
            current_num++;
        }
    }
}

void station ::setqueque_person(person **persons, int minute)
{
    for (int i = 0; i < personnum[minute]; i++)
    {
        int min = 0;
        for (int m = 0; m < stationnum; m++)
        {
            if (tag[m] < tag[min])
            {
                min = m;
            }
        }
        if (tag[min] == 10)
        {
            stationnum++;
            person_queue[stationnum - 1][0] = persons[minute][i];
            tag[stationnum - 1]++;
        }
        else
        {
            person_queue[min][tag[min]] = persons[minute][i];
            tag[min]++;
        }
    }
}

void station ::printqueue_person(int minute)
{
    cout << "Current information of common collection points for people:" << endl;
    for (int i = 0; i < stationnum; i++)
    {
        cout << "This is normal station " << i + 1 << ":" << endl;
        for (int j = 0; j < tag[i]; j++)
        {
            cout << j + 1 << ". ID: " << person_queue[i][j].getID() << ",  age: " << person_queue[i][j].getage() << ", gender: " << person_queue[i][j].getgender() << endl;
        }
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << endl;
        if (tag[i] >= 5)
        {
            for (int k = 0; k < tag[i] - 5; k++)
            {
                person_queue[i][k] = person_queue[i][k + 5];
            }
            tag[i] -= 5;
        }
        else
        {
            tag[i] = 0;
        }
    }
}

void station ::print(police **polices, person **persons)
{
    int current_num = 0;
    int total = 0;
    setqueque_police(polices);
    for (int i = 0; i < 30; i++)
    {
        cout << "This is the nucleic acid detection station in the " << i + 1 << "minute: " << endl;
        cout << "-----------------------------------------------------------------------------------------" << endl;
        printqueue_police(i, current_num, total);
        cout << "***************************************************************************************************" << endl;
        setqueque_person(persons, i);
        printqueue_person(i);
        cout << "-----------------------------------------------------------------------------------------" << endl;
        cout << endl;
    }
}

void randomdata(long **ID1, long **ID2, int **policeID, station cov_station) // Generate distinct random ID data
{
    srand((unsigned)time(0));
    for (int i = 0; i < 30; i++)
    {
        long a[100001];
        int b[1001];

        ID1[i] = new long[50];
        ID2[i] = new long[50];
        policeID[i] = new int[50];

        for (long k = 0; k < 100001; ++k) // Randomly generate the personID within 100000~20000
            a[k] = k;                     // Store integers in the numerical range in an array
        for (long j = 100000; j >= 1; --j)
            swap(a[j], a[rand() % j]); // Exchange the value of randomly generated index footer with the array value
        for (int m = 0; m < cov_station.personnum[i]; m++)
        {
            ID1[i][m] = a[m] + 100000;
        }
        for (int n = 0; n < cov_station.policenum[i]; n++)
        {
            ID2[i][n] = a[n + cov_station.personnum[i]] + 100000;
        }

        for (int k = 0; k < 1001; ++k) // Randomly generate the policeID within 1000~2000
        {
            b[k] = k;
        }
        for (int j = 1000; j >= 1; --j)
        {
            swap(b[j], b[rand() % j]);
        }
        for (int m = 0; m < cov_station.policenum[i]; m++)
        {
            policeID[i][m] = b[m] + 1000;
        }
    }
}

void set_age_gender(int **age1, string **gender1, int **age2, string **gender2, station cov)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < 30; i++)
    {
        age1[i] = new int[50];
        age2[i] = new int[50];
        gender1[i] = new string[50];
        gender2[i] = new string[50];
        for (int j = 0; j < cov.personnum[i]; j++)
        {
            age1[i][j] = 1 + rand() % 150;
            int a = rand() % 2;
            if (a == 0)
                gender1[i][j] = "female";
            else
                gender1[i][j] = "male";
        }
        for (int j = cov.personnum[i]; j < 50; j++)
        {
            age2[i][j - cov.personnum[i]] = 1 + rand() % 150;
            int a = rand() % 2;
            if (a == 0)
                gender2[i][j - cov.personnum[i]] = "female";
            else
                gender2[i][j - cov.personnum[i]] = "male";
        }
    }
}

int main()
{
    station cov_station;
    long **ID1 = new long *[30];    // Used to store the ID of each ordinary person
    long **ID2 = new long *[30];    // Used to store the ID of each police
    int **policeID = new int *[30]; // Used to store the police ID of each police
    int **person_age = new int *[30];
    int **police_age = new int *[30];
    string **person_gender = new string *[30];
    string **police_gender = new string *[30];
    randomdata(ID1, ID2, policeID, cov_station);
    set_age_gender(person_age, person_gender, police_age, police_gender, cov_station);

    police **polices = new police *[30];
    for (int i = 0; i < 30; i++)
    {
        polices[i] = new police[50];
        for (int j = 0; j < cov_station.policenum[i]; j++)
        {
            polices[i][j].setdata2(ID2[i][j], policeID[i][j], police_age[i][j], police_gender[i][j]);
        }
    }

    person **persons = new person *[30];
    for (int i = 0; i < 30; i++)
    {
        persons[i] = new person[50];
        for (int j = 0; j < cov_station.personnum[i]; j++)
        {
            persons[i][j].setdata1(ID1[i][j], person_age[i][j], person_gender[i][j]);
        }
    }

    cov_station.print(polices, persons);
}
