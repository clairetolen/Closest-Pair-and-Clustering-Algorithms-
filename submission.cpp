#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>

using namespace std;

struct Point{
    int x;
    int y;
    int index;
};
struct Point3D{
    int x;
    int y;
    int z;
    int index;
};


void mergesort(vector <Point>& points, int l, int r, bool byX);
void merge(vector<Point>& points, int l, int m, int r, bool byX);
double bruteForce(vector <Point> points, int n, int &minIndex1,int &minIndex2);
double divideAndConquer(vector<Point> points, int left, int right, int &minIndex1, int &minIndex2);
void clusterPoints(vector<Point3D> points, ofstream&output);

double distance(Point& p1, Point& p2){ ///pass in points by reference
    double x1 = p1.x;
   double  x2 = p2.x;
    double y1 = p1.y;
    double y2 = p2.y; 
   double d = sqrt(pow((x1-x2),2)+pow((y1-y2),2));
    return d;

}

int main (int argc, char* argv[]){
   
    ifstream input (argv[1]);
    ofstream output(argv[2]);

    vector<Point> points; //vector that stores all points from input
    vector<Point3D> points3D; ////vector for 3D inputs

    string line;
    int index = 0;


    while (getline(input, line)){ ///read input line and determine which vector to add to

        stringstream ss(line);
        int x, y, z;

        if (ss >> x>> y){
            if (ss >> z){
                Point3D p = {x,y,z, index};
                points3D.push_back(p);
            }else {
                Point p = {x,y,index}; //first input is x, next is y --> store its index as well in the point
                points.push_back(p);  //store in vector 

            }
            index ++;
        }

    }
    cout << "2D points: " << points.size() << endl;

    cout << "3D points: " << points3D.size() << endl;

     //////CALL PART B4 - if 3D points array exits w elements
    if (points3D.size()> 0){ 
        clusterPoints(points3D, output);
        return 0; ///so that other 2D alg dont run
    }

    int bruteIndex1;
    int bruteIndex2;

    auto bruteStart = chrono::high_resolution_clock::now(); 
    double bruteAnswer = bruteForce(points, index, bruteIndex1, bruteIndex2);
    auto bruteEnd = chrono::high_resolution_clock::now(); 


    chrono::duration<double,milli> bruteTime = bruteEnd-bruteStart;

    vector<Point> ogPoints = points; //save for printing later

    ///sort vector (for D&C)
    //put smallest x coordinate first
    mergesort(points,0, points.size() -1, true);

    int dcIndex1;
    int dcIndex2;

    auto dcStart = chrono::high_resolution_clock::now(); 
    double dcAnswer = divideAndConquer( points, 0,  index -1, dcIndex1, dcIndex2);
    auto dcEnd = chrono::high_resolution_clock::now(); 

    chrono::duration<double,milli> dcTime = dcEnd-dcStart;

   output << "/////////// BRUTE FORCE ///////////"  << endl;
   output << endl;
    output<< "Closest Pair: (" << ogPoints[bruteIndex1].x <<", "<< ogPoints[bruteIndex1].y<< ") and ("<<ogPoints[bruteIndex2].x << ", "<<ogPoints[bruteIndex2].y << ")"<<endl;
   output <<"Distance: " << bruteAnswer << endl;
   output << endl;
   output<< "Index 1: "<<  bruteIndex1 << endl;
   output<< "Index 2: " << bruteIndex2 <<endl;

    output << endl;
    output << "Time: " << bruteTime.count()<< " ms" << endl;
    output << endl;

    output << "/////////// DIVIDE AND CONQUER ///////////" <<  endl;
    output << endl;
     output<< "Closest Pair: (" << ogPoints[dcIndex1].x <<", "<< ogPoints[dcIndex1].y<< ") and ("<<ogPoints[dcIndex2].x << ", "<<ogPoints[dcIndex2].y << ")"<<endl;
        output << "Distance: " << dcAnswer << endl;
    output << endl;
    output << "Index 1: " << dcIndex1 <<endl;
    output << "Index 2: " << dcIndex2 << endl;
    output << endl;
    output << "Time: " << dcTime.count() << " ms" << endl;
    output << endl;


    ////Time comparison///
    output << "/////////// TIME COST COMPARISON ///////////" <<  endl;
    output << endl;

    if (dcTime.count()<bruteTime.count() ){
        output << "Faster Algorithm: Divide and Conquer" << endl;
    }else if (bruteTime.count() < dcTime.count()){
        output << "Faster Algorithm: Brute Force" << endl;
    }else{
        output << "Brute Force and Divide and Conquer speed equal." << endl;
    }
    
 
 }

double bruteForce(vector <Point> points, int n, int &minIndex1,int &minIndex2){  //pass by reference --> allows modification of OG

    //tracking
    double minDist = 9999999;
   


    for (int i = 0; i < n -1; i++){
        for (int j = i +1; j <n; j++){
           Point p1 = points[j];
           Point p2 = points[i];
           
          double d = distance(p1, p2);

        if (d < minDist){
            minIndex1 = points[i].index;
            minIndex2 = points[j].index;
            minDist = d;
        }
    }
    }
    return minDist;

}

double divideAndConquer(vector<Point> points, int left, int right, int &minIndex1, int &minIndex2){
    int mid = (left + right)/2;

    //base case
    if (right - left +1 <= 3){ //exactly two points
        //stop dividing and calculate distance between each
       
        double minDist = 999999999;
        for (int i = left; i <right; i++){
            for (int j = i +1; j <= right; j++){
                
                double d = distance(points[i], points[j]);
            

            if (d < minDist){
                minIndex1 = points[i].index;
                minIndex2 = points[j].index;
                minDist = d;
                 }
            }
        }
    return minDist;
    }

    //SOLVE LEFT HALF///

    int leftIndex1, leftIndex2;
    double leftDist = divideAndConquer(points, left, mid, leftIndex1, leftIndex2);

    ///SOLVE RIGHT HALF////
    int rightIndex1, rightIndex2;
    double rightDist = divideAndConquer(points, mid +1, right, rightIndex1, rightIndex2);

    double minDist = 9999999;
 
    if (leftDist < rightDist){ //if left side has shorter path distance
        minDist = leftDist;

        minIndex1 = leftIndex1;
        minIndex2 = leftIndex2;
    }else { ///if right path is shorter
          minDist = rightDist;

        minIndex1 = rightIndex1;
        minIndex2 = rightIndex2;
    }

    ///CHECK MIDDLE - aka between groups/// 

    double midX = points[mid].x;

    vector <Point> middle; //vector for points close to divide
    for (int i = left; i <= right; i++){
        if (abs(points[i].x -midX) < minDist){
            middle.push_back(points[i]);
        }
    }


    ///sort by y

   mergesort(middle, 0, middle.size() -1, false);
    for (int i = 0; i < middle.size(); i++){
        for (int j = i +1; j <middle.size(); j++){
            if (middle[j].y - middle[i].y >= minDist){
                break; //end search once points are at least minDist apart VERTICALLY (all others will be farther than min)
            }
            double d = distance(middle[i], middle[j]);

            if (d < minDist){
                minDist = d;

                minIndex1 = middle[i].index;
                minIndex2 = middle[j].index;
            }

        }
    }
            /*
            for (int i = 0; i <middle.size(); i++){
                for (int j = i + 1; j <middle.size(); j++){
                    double d = distance (middle[i], middle[j]);

                    if (d < minDist){
                        minDist = d;

                        minIndex1 = middle[i].index;
                        minIndex2 = middle[j].index;
                    }
                }

            }
            */

    return minDist;
}



/////PART B4////////

double distance3D(Point3D& p1, Point3D& p2){
    double x1 = p1.x;
    double y1 = p1.y;
    double z1 = p1.z;

    double x2 = p2.x;
    double y2 = p2.y;
    double z2 = p2.z;

    double d = sqrt(pow(x1-x2, 2)+ pow((y1-y2),2)+ pow((z1 - z2),2));
    return d;

}

void clusterPoints(vector<Point3D> points, ofstream& output){
    Point3D center1 = points[0];  ///first point is center 1
    Point3D center2 = points[points.size()/2]; ///middle point is center 2
    Point3D center3 = points[points.size()-1]; ///last point = center 3

    vector<Point3D> group1;
    vector<Point3D> group2;
    vector<Point3D> group3;

    for (int i = 0; i <100; i++){ //iterations
        group1.clear();
        group2.clear();
        group3.clear();

        for (int j = 0; j < points.size(); j++){ // j = point we are on in each iteration
            
            ///compare point j to all 3 centers//
            double d1 = distance3D(points[j], center1);
            double d2 = distance3D(points[j], center2);
            double d3 = distance3D(points[j], center3);

            //determine which group to add point j to
            ///add to group such that point has min distance to center
            if (d1 <= d2 && d1 <= d3){
                group1.push_back(points[j]);

            } else if (d2 <= d1 && d2 <= d3){
                group2.push_back(points[j]);
            }else{ 
                group3.push_back(points[j]);
            }

        }

        double sumX = 0;
        double sumY = 0;
        double sumZ = 0;

        //add up coordinates
        for (int j = 0; j < group1.size(); j++){
            sumX += group1[j].x;
             sumY += group1[j].y;
            sumZ += group1[j].z;
        }
        ///change center by finding average of group 1
        if (group1.size()>0){
            center1.x = sumX / group1.size();
            center1.y = sumY / group1.size();
            center1.z = sumZ / group1.size();
        }


        ///repeat for groups 2 and 3   //////
         sumX = 0;
        sumY = 0;
        sumZ = 0;
        for (int j = 0; j < group2.size(); j++){
            sumX += group2[j].x;
             sumY += group2[j].y;
            sumZ += group2[j].z;
        }

        if (group2.size()>0){
            center2.x = sumX / group2.size();
            center2.y = sumY / group2.size();
            center2.z = sumZ / group2.size();
        }

        sumX = 0;
        sumY = 0;
        sumZ = 0;
        for (int j = 0; j < group3.size(); j++){
            sumX += group3[j].x;
             sumY += group3[j].y;
            sumZ += group3[j].z;
        }

        if (group3.size()>0){
            center3.x = sumX / group3.size();
            center3.y = sumY / group3.size();
            center3.z = sumZ / group3.size();
        }

   
    }


    output<< "GROUP 1" <<endl;
    for (int i = 0; i < group1.size(); i++){
        output<<group1[i].x << " " << group1[i].y << " "<< group1[i].z<< endl;
    }
    output << endl;


    output<< "GROUP 2" <<endl;
    for (int i = 0; i < group2.size(); i++){
        output<<group2[i].x << " " << group2[i].y << " "<< group2[i].z<< endl;
    }
    output<<endl;

    output<< "GROUP 3" <<endl;
    for (int i = 0; i < group3.size(); i++){
        output<<group3[i].x << " " << group3[i].y << " "<< group3[i].z<< endl;
    }
    output << endl;

   
}
/////////////////////////////////////////////////////////////////////

void merge(vector<Point>& points, int l, int m, int r, bool byX){

    vector<Point> temp;
    int i = l; ///pointer to left half 
    int j = m +1;   /// p to right half

    while (i <= m && j <= r){
        if (byX){
            if (points[i].x < points[j].x){
                temp.push_back(points[i]);
                i++;
            }else{
                temp.push_back(points[j]);
                j++;
            }
        }else{ 
            if (points[i].y < points[j].y){
                temp.push_back(points[i]);
                i++;
            }else{
                temp.push_back(points[j]);
                j++;
            }
        }
    }

    while(i <= m ){
        temp.push_back(points[i]);
        i++;
    }
     while(j <= r ){
        temp.push_back(points[j]);
        j++;
    }


    ///copy temp back
    for (long unsigned int x = 0; x <temp.size(); x++){
        points[l + x] = temp[x];
    }



}

void mergesort(vector <Point>& points, int l, int r, bool byX){

    if (l < r){
        int mid = l + (r - l)/2;
        mergesort(points,l, mid, byX);
        mergesort(points, mid +1, r, byX);

        merge(points, l, mid, r, byX);
    }
    
}

