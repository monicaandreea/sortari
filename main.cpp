#include<iostream>
#include<vector>
#include<chrono>
#include<algorithm>
#include<fstream>
using namespace std;

ifstream f("date.in");
ofstream g("date.out");

//generating numbers
vector<int> teste( int &n, int &m)
{
    int i;
    vector<int> v;
    srand( time(0));

    //n = rand() % 10000000 + 1; // genereaza numarul de numere
    //n=10000;
    //m=1000000;
    for( i=0 ; i<n ; i++)
        v.push_back( rand() %m ); //genereaza numere mai mici ca 10^4

    return v;
}

vector<int> teste_descrescator( int &n, int &m)
{
    int i;
    vector<int> v;
    srand( time(0));

    //n = rand() % 10000000 + 1; // genereaza numarul de numere
    //n=10000;
    //m=1000000;
    for( i=0 ; i<n ; i++)
        v.push_back( rand() %m ); //genereaza numere mai mici ca 10^4
    sort(v.begin(), v.end(), greater<int>());
    return v;
}

vector<int> teste_crescator( int &n, int &m)
{
    int i;
    vector<int> v;
    srand( time(0));

    //n = rand() % 10000000 + 1; // genereaza numarul de numere
    //n=10000;
    //m=1000000;
    for( i=0 ; i<n ; i++)
        v.push_back( rand() %m ); //genereaza numere mai mici ca 10^4
    sort(v.begin(), v.end());
    return v;
}

//Bubble sort
//interschimbarea elementelor adiacente pana ajung in ordinea buna(crescatoare)
vector<int> BubbleSort(vector<int> v, int n)
{
    auto start = chrono::high_resolution_clock::now();
    int i, j, intrerupt = 0;
    for( i=0 ; i<n-1 ; i++)
        for( j=0 ; j<n-i-1 ; j++)
        {
            auto timp = chrono::high_resolution_clock::now();
            auto durata = chrono::duration_cast<chrono::microseconds>(timp - start);
            if( durata.count() / 1000 > 60000){intrerupt = 1; break;}
            if( v[j] > v[j+1] )
                swap(v[j], v[j+1]);
        }

    if(intrerupt==1) v[0]=-55;
    return v;
}

//Counting sort
//vector de frecventa
vector<int> CountingSort(vector<int> v, int n, int m)
{
    int i;
    vector<int> frecventa(m, 0); //vector de lungime m(cel mai mare numar)

    for( i=0 ; i<n ; i++)
    {
        frecventa[v[i]]++;
        v[i] = 0;
    }

    int j=-1;
    for( i=0 ; i<m ; i++)
    {
        while(frecventa[i] > 0)
        {
            j++;
            v[j] = i;
            frecventa[i]--;
        }
    }

    return v;
}

//Radix Sort
//sorting by digits (starting with least significant)
int gasesteMaxim(vector<int> v, int n)
{
    int i, maxim = v[0];
    for( i=1 ; i<n ; i++)
        if( v[i] > maxim) maxim = v[i];

    return maxim;
}

vector<int> CountingSort_Radix(vector<int> v, int n, int m)
{
    int i;
    vector<int> frecventa(10, 0), rezultat(n, 0); //vector de lungime 10 pt 0,1,2...9

    for( i=0 ; i<n ; i++)
        frecventa[(v[i] / m)%10]++; //a m-a litera de la dreapta la stanga

    for( i=1 ; i<10 ; i++)
        frecventa[i] += frecventa[i-1]; //adaugam la poz curenta, cea anterioara pnetru a avea indicii din vector

    for( i=n-1 ; i>=0; i--) //luam de la ultimul la primul ("cel mai mare" -> "cel mai mic")
    {
        rezultat[frecventa[(v[i]/m)%10] -1] = v[i];
        //punem in rez nr curent, pe poz indicata de a m-a litera de la stanga la dreapta (-1 pentru ca incepem de la 0)
        frecventa[(v[i]/m)%10]--; //scadem frecventa => scade pozitia
    }

    for( i=0 ; i<n ; i++)
        v[i] = rezultat[i];

    return v;
}

vector<int> RadixSort(vector<int> v, int n)
{
    int i, maxim = gasesteMaxim(v, n); //maximul dintre numere ca sa stim numarul maxim de cifre

    for( i=1 ; maxim/i>0 ; i=i*10) // i reprezinta o putere a lui 10 la care impartim pt a gasi a i-a litera de la stanga la dreapta
        v= CountingSort_Radix(v, n, i);

    return v;
}

//Merge sort
//divide et impera + interclasare intre left si right

void combining(vector<int> &v, int l, int m, int r)
{
    int i;
    int n_left = m-l+1; //lung jumatatii din staga/dreapta
    int n_right = r-m;

    int left[n_left], right[n_right];

    for( i=0 ; i<n_left ; i++) //copiem jumatatiile in vectorii respectivi
        left[i] = v[l+i];
    for( i=0 ; i<n_right ; i++)
        right[i] = v[m+1+i];

    i = 0; //indice matrice left
    int j = 0; //indice matrice right
    int k = l; //indice matrice v (incepe de la l)

    while( i<n_left && j<n_right ) //interclasare
    {
        if( left[i] <= right[j] ) {v[k] = left[i]; i++;}
        else {v[k] = right[j]; j++;}
        k++;
    }

    while( i<n_left ) v[k] = left[i], i++, k++;
    while( j<n_right ) v[k] = right[j], j++, k++;

}

void MergeSort(vector<int> &v, int l, int r)
{
    if(l>=r) return;
    int m = l+(r-l)/2;
    MergeSort(v, l, m);
    MergeSort(v, m+1, r);
    combining(v, l, m, r);
}

//Quick sort
// divide et impera cu pivot pe ultimul element

int partitie(vector<int> &v, int l, int r)
{
    int pivot = v[r]; //ultimul element
    int i, poz = l - 1;

    for (i = l; i <= r; i++)
        if (v[i] < pivot) {
            poz++;
            swap(v[i], v[poz]);
        }

    swap(v[poz + 1], v[r]);
    return poz + 1;
}

void QuickSort_pivot_ultimul(vector<int> &v, int l, int r)
{
    if(l<r)
    {
        int p = partitie(v, l, r);
        QuickSort_pivot_ultimul(v, l, p-1);
        QuickSort_pivot_ultimul(v, p+1, r);
    }
}

//Quick sort
// divide et impera cu pivot din mediana de 3

void mediana(vector<int> &v, int l, int r)
{
    int pivot, mid = (l+r)/2;
    if((v[l]<= v[mid] && v[mid] <= v[r]) || (v[r]<= v[mid] && v[mid] <= v[l]))
        pivot = mid;
    if((v[mid]<= v[l] && v[l] <= v[r]) || (v[r]<= v[l] && v[l] <= v[mid]))
        pivot = l;
    if((v[l]<= v[r] && v[r] <= v[mid]) || (v[mid]<= v[r] && v[r] <= v[l]))
        pivot = r;

    swap(v[pivot], v[r]); //punem pe ultima pozitie ca sa putem aplica acelasi cod ca la primul quicksort
}

int partitie_m(vector<int> &v, int l, int r)
{
    mediana(v, l, r);
    int pivot = v[r];
    int i, poz = l - 1;

    for (i = l; i <= r; i++)
        if (v[i] < pivot) {
            poz++;
            swap(v[i], v[poz]);
        }

    swap(v[poz + 1], v[r]);
    return poz + 1;
}

void QuickSort_pivot_mediana(vector<int> &v, int l, int r)
{
    if(l<r)
    {
        int p = partitie_m(v, l, r);
        QuickSort_pivot_mediana(v, l, p-1);
        QuickSort_pivot_mediana(v, p+1, r);
    }
}

//other stuff
void afisare(vector<int> v, int n)
{
    int i;
    //g<<endl<<"Vectorul initial "<<endl;
    for( i=0 ; i<n ; i++)
        g<<v[i]<<" ";
    g<<endl;
}

bool verificare(vector<int> v, vector<int> sortat, int n)
{
    int i;
    /* for( i=0 ; i<n ; i++)
         g<<v[i]<<" "; */
    sort(v.begin(), v.end());
    for( i=0 ; i<n ; i++)
        if( sortat[i] != v[i] ) return 0;

    return 1;
}

int main()
{
    int t;
    f>>t;
    while(t--)
    {
        int n, m, o;
        f>>n>>m>>o;
        vector <int> v, bs, cs, rs, ms, qsu, qsm;
        if(o==1)v = teste( n, m );
        else if(o==2)v = teste_descrescator( n, m );
        else if(o==3) v = teste_crescator( n, m );

        for( int i = 0; i < n; ++i )
        {ms.push_back(v[i]); qsu.push_back(v[i]); qsm.push_back(v[i]); }

        g<<"n = "<<n<<endl<<"m = "<<m<<endl;

        g<<"Sortarea din c++: ";
        vector <int> sortat= v;
        auto start = chrono::high_resolution_clock::now();
        sort( sortat.begin(), sortat.end() );
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        g << duration.count() / 1000 << "ms\n";

        g<<"Counting sort: ";
        start = chrono::high_resolution_clock::now();
        cs = CountingSort(v, n, m);
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        g << duration.count() / 1000 << "ms\n";
        if( verificare(v, cs, n) == 1) g<<"Vectorul a fost sortat corect.";
        else g<<"Vectorul a fost sortat gresit.";
        //afisare(cs, n);

        g<<endl<<"Radix sort: ";
        start = chrono::high_resolution_clock::now();
        rs = RadixSort(v, n);
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        g << duration.count() / 1000 << "ms\n";
        if( verificare(v, cs, n) == 1) g<<"Vectorul a fost sortat corect.";
        else g<<"Vectorul a fost sortat gresit.";
        //afisare(rs, n);

        g<<endl<<"Merge sort: ";
        if(n>500000) g<<"prea multe numere, risc de stack overflow"<<endl;
        else{
            start = chrono::high_resolution_clock::now();
            MergeSort(ms,0, n-1);
            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::microseconds>(stop - start);
            g << duration.count() / 1000 << "ms\n";
            if( verificare(v, ms, n) == 1) g<<"Vectorul a fost sortat corect.";
            else g<<"Vectorul a fost sortat gresit.";
            g<<endl;
            //afisare(ms, n);
        }

        g<<"Quick sort cu pivot pe ultimul element: ";
        start = chrono::high_resolution_clock::now();
        QuickSort_pivot_ultimul(qsu,0, n-1);
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        g << duration.count() / 1000 << "ms\n";
        if( verificare(v, qsu, n) == 1) g<<"Vectorul a fost sortat corect.";
        else g<<"Vectorul a fost sortat gresit.";
        //afisare(qsu, n);

        g<<endl<<"Quick sort cu pivot din mediana de 3: ";
        start = chrono::high_resolution_clock::now();
        QuickSort_pivot_mediana(qsm,0, n-1);
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        g << duration.count() / 1000 << "ms\n";
        if( verificare(v, qsm, n) == 1) g<<"Vectorul a fost sortat corect.";
        else g<<"Vectorul a fost sortat gresit.";
        //afisare(qsm, n);

        g<<endl<<"Bubble sort: ";
        start = chrono::high_resolution_clock::now();
        bs = BubbleSort(v, n);
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        g << duration.count() / 1000 << "ms\n";
        if(bs[0] == -55) g<<"Sortarea a fost oprita dupa 1 minut.";
        else if( verificare(v, bs, n) == 1) g<<"Vectorul a fost sortat corect.";
        else g<<"Vectorul a fost sortat gresit.";
        g<<endl<<endl;
        //afisare(bs, n);
        //afisare(v, n);
    }

    return 0;
}