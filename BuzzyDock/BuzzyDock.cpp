/******************************************************************************
* Author: Qiaoling Chen
* Class: ECE6122
* Description: This is the MPI program that takes Yellow Jackets back to Buzzy
  and avoding crashes as much as I can. As all thruster are unstable, it dangerous
  to calculate a precise maximum velocity based on distance, thus all such max_v
  and min_v are hardcoded, in my tests, they meet the requirements and avoides crashing
******************************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#define  MASTER		0
using namespace std;

int main(int argc, char *argv[])
{
    int   numtasks, taskid, len, rc;
    char hostname[MPI_MAX_PROCESSOR_NAME];

//======================================== initialization ========================================
    // store the phase of different YJs
    int phase[7];
    for(int i=0; i<7; i++)
    {
        // initialize each phase to 0 state
        phase[1] = 0;
    }

    int totalTime;
    double a_max;

    // store Buzzy's position, velocity, direction in arrays
    double BuzzyPos[3];
    double BuzzyVel;
    double BuzzyDir[3];
    int BuzzyStat = 1;


    double v_max = 80;

    // store Yellow Jackets' position, velocity, direction, thrust action in arrays
    double **YJPos;
    YJPos = new double*[7];
    for(int i=0; i<7; i++)
    {
        YJPos[i] = new double[3];
    }

    double YJVel[7];

    double **YJDir;
    YJDir = new double*[7];
    for(int i=0; i<7; i++)
    {
        YJDir[i] = new double[3];
    }

    // initialize the acceleration created by thrusts of YJ, the initial state is 0 as all thrusts are closed
    double **YJacc;
    YJacc = new double*[7];
    for(int i=0; i<7; i++)
    {
        YJacc[i] = new double[3];
    }

    // mess of Yellow Jackets
    double mess = 10000;

    // distances that can matter
    double dockDis = 80;
    double crushBuzz = 50;
    double crushYJ = 250;

    // initialize Yellow Jackets' status, all YJs are functional, undocked, uncrashed
    int YJStatus[7];

    
    int signal = 1;
    double wait[7];
//======================================== initialization =========================================
    

    MPI_Init(&argc, &argv);
    /*
     this code will be run on every single distinguished processor
    */
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Get_processor_name(hostname, &len);


    if (taskid == MASTER)
    {
        /* 
        This is the main processor, it calculates the new locations of Buzzy per sec and gathers and broadcasts the location
        of Buzzy and the seven YJs
        */

        // read in the file, store the information
        string path = "in.dat";
        ifstream infile;
        infile.open(path.c_str());
        infile >> totalTime;
        infile >> a_max;
        a_max /= mess;
        for(int i=0; i<3; i++)
        {
            infile >> BuzzyPos[i];
        }
        infile >> BuzzyVel;
        for(int i=0; i<3; i++)
        {
            infile >> BuzzyDir[i];
        }
        for(int i=0; i<7; i++)
        {
            for(int j=0; j<3; j++)
            {
                infile >> YJPos[i][j];
            }
            infile >> YJVel[i];
            for(int j=0; j<3; j++)
            {
                infile >> YJDir[i][j];
            }
        }
        infile.close(); // finish reading file

        // send the maximum acceleration information to each YJs so that they can ajust the flying strategy
        // the number is fixed so that only to be sent onece
        for(int i=0; i<7; i++) 
        {
            MPI_Send(&a_max, 1, MPI_DOUBLE, i + 1, 0, MPI_COMM_WORLD);
            MPI_Send(&totalTime,1,MPI_INT,i+1,0,MPI_COMM_WORLD);
        }

        for(int i=0; i<7;i++)
        {
            for(int j=0;j<3;j++)
            {
                YJacc[i][j] = 0;
            }
        }

        for (int i=0; i<7; i++)
        {
            YJStatus[i] = 1;
        }

    }

    else
    {
        /*
            These are the other processors. In this step they only receive the 
            maximum acceleration information
        */
        MPI_Status status;
        MPI_Recv(&a_max, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&totalTime,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
    }

// =============================================Start docking ============================================
    for(int sec=0; sec<totalTime; sec++)
    {
        // docking within given time, each step represents one second.
        if(taskid == MASTER)
        {
            /*
            This is the main processor, we calculate the location of Buzzy and 
            broadcase the status and location of itself and all other seven YJs
            */
            for(int i=0; i<7; i++)
            {

                // broadcast Buzzy's status and location to all seven yellow jackets
                MPI_Send(BuzzyPos, 3, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD);
                MPI_Send(BuzzyDir, 3, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD);
                MPI_Send(&BuzzyVel, 1, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD);

                // broadcase YJ's location
                for(int j=0; j<7;j++)
                {
                    MPI_Send(YJPos[j], 3, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD);
                    MPI_Send(YJDir[j], 3, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD);
                    MPI_Send(&YJVel[j], 1, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD);
                    MPI_Send(YJacc[j], 3, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD);

                }
                MPI_Send(YJStatus, 7, MPI_INT, i+1,0,MPI_COMM_WORLD);  
            }

            // calculate the Buzzy's next position
            BuzzyPos[0] += BuzzyVel * BuzzyDir[0];
            BuzzyPos[1] += BuzzyVel * BuzzyDir[1];
            BuzzyPos[2] += BuzzyVel * BuzzyDir[2];

            // receive the location, status, velocity and direction of YJs
            MPI_Status status;
            for(int j=0; j<7; j++)
            {

                if(YJStatus[j] ==0) 
                {
                    // for crashed YJs, their location is where they were crashed, which is the sent to Buzzy the last time. 
                    // to save time, we make Buzzy stop receiving information from destroyed YJs
                	printf("%d, %d, %.6e, %.6e, %.6e, %.6e, %.6e, %.6e\n", j+1, YJStatus[j], YJPos[j][0], YJPos[j][1], YJPos[j][2], YJacc[j][0],YJacc[j][1],YJacc[j][2]);
                	continue;
                }

                if(YJStatus[j] ==2)
                {
                    // for already docked YJs, their locations are the same as Buzzy's.
                    // to save time, we directly output the Buzzy's location and refuse to receive information from docked YJs
                	printf("%d, %d, %.6e, %.6e, %.6e, %.6e, %.6e, %.6e\n", j+1, YJStatus[j], BuzzyPos[0], BuzzyPos[1], BuzzyPos[2], YJacc[j][0],YJacc[j][1],YJacc[j][2]);
                	continue;
                }

                // receive information from active YJs
                MPI_Recv(&YJStatus[j], 1, MPI_INT, j+1, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(YJPos[j], 3, MPI_DOUBLE, j+1, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(&YJVel[j], 1, MPI_DOUBLE, j+1, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(YJDir[j], 3, MPI_DOUBLE, j+1, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(YJacc[j], 3, MPI_DOUBLE, j+1, 0, MPI_COMM_WORLD, &status);
                printf("%d, %d, %.6e, %.6e, %.6e, %.6e, %.6e, %.6e\n", j+1, YJStatus[j], YJPos[j][0], YJPos[j][1], YJPos[j][2], YJacc[j][0],YJacc[j][1],YJacc[j][2]);
                //}
            }

        }
        else
        {
            /*
            These are the yellow jackets' processers. A YJ receives the broadcast from
            Buzzy and adjust its flying direction
            */
            MPI_Status status;

            // initialize variables to store renewed information received from Buzzy each time
            double BuzzyPosition[3], BuzzyVelocity, BuzzyDirection[3];
            double **YJPosition;
            YJPosition = new double*[7];
            for(int i=0; i<7; i++)
            {
                YJPosition[i] = new double[3];

            }
            double YJVelocity[7];

            double **YJDirection;
            YJDirection = new double*[7];
            for(int i=0; i<7; i++)
            {
                YJDirection[i] = new double[3];
            }

            double **YJacceleration;
            YJacceleration = new double*[7];
            for(int i=0; i<7; i++)
            {
                YJacceleration[i] = new double[3];
                
            }


            // receive information from Buzzy
            // receive Buzzy's location, direction, velocity
            MPI_Recv(BuzzyPosition, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(BuzzyDirection, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&BuzzyVelocity, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            for (int k=0; k<7; k++)
            {
                // receive other YJs' locations, directions, velocities from Buzzy
                MPI_Recv(YJPosition[k], 3, MPI_DOUBLE, 0,0,MPI_COMM_WORLD, &status);
                MPI_Recv(YJDirection[k], 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(&YJVelocity[k], 1, MPI_DOUBLE,0,0,MPI_COMM_WORLD, &status);
                MPI_Recv(YJacceleration[k],3,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
            }
            MPI_Recv(YJStatus,7,MPI_INT,0,0,MPI_COMM_WORLD,&status);

            // calculate the relative velocity
            double curRelaVelo[3];
            curRelaVelo[0] = YJVelocity[taskid-1] * YJDirection[taskid-1][0] - BuzzyVelocity * BuzzyDirection[0];
            curRelaVelo[1] = YJVelocity[taskid-1] * YJDirection[taskid-1][1] - BuzzyVelocity * BuzzyDirection[1];
            curRelaVelo[2] = YJVelocity[taskid-1] * YJDirection[taskid-1][2] - BuzzyVelocity * BuzzyDirection[2];
            double relaV = pow((pow(curRelaVelo[0],2)+pow(curRelaVelo[1],2)+pow(curRelaVelo[2],2)),0.5);
            
            // calculate current location
            double curRelaPos[3];
            curRelaPos[0] = YJPosition[taskid-1][0] - BuzzyPosition[0];
            curRelaPos[1] = YJPosition[taskid-1][1] - BuzzyPosition[1];
            curRelaPos[2] = YJPosition[taskid-1][2] - BuzzyPosition[2];
            double curRelaDis = pow(pow(curRelaPos[0],2)+pow(curRelaPos[1],2)+pow(curRelaPos[2],2),0.5);

            //========================= check if this YJ will crash ================================
            int crash = 0;
            // check distance with other YJs
            for(int i=0; i<7; i++)
            {
                // check if crashes with other YJs

                // avoid itself and already crashed or docked YJs
                if(i == taskid-1 || YJStatus[taskid-1] == 0 || YJStatus[taskid-1] == 2) continue; 

                // calculate the distance
                double YJDis = pow(pow(YJPosition[taskid-1][0]-YJPosition[i][0],2)+pow(YJPosition[taskid-1][1]-YJPosition[i][1],2)+pow(YJPosition[taskid-1][2]-YJPosition[i][2],2),0.5);
                if(YJDis < 250)
                {

                    crash = 1;
                    YJStatus[taskid-1] = 0;
                    break;
                    
                }
            }
            
            //check if crashes with Buzzy
            /*
            if(curRelaDis <= 50) {
                double cos_theta = YJDirection[taskid-1][0]*BuzzyDirection[0]+YJDirection[taskid-1][1]*BuzzyDirection[1]+YJDirection[taskid-1][2]*BuzzyDirection[2];
                if(cos_theta<=0.8 || YJVelocity[taskid-1]>=1.1*BuzzyVelocity) {
                    crash = 1;
                    YJStatus[taskid-1] = 0;
                }
            }
*/

            // if crashes, send the information back to Buzzy and break the loop
            
            if(crash == 1)
            {
                YJacceleration[taskid-1][0] = 0;
                YJacceleration[taskid-1][1] = 0;
                YJacceleration[taskid-1][2] = 0;

                MPI_Send(&YJStatus[taskid-1], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Send(YJPosition[taskid-1], 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                MPI_Send(&YJVelocity[taskid-1], 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                MPI_Send(YJDirection[taskid-1], 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                MPI_Send(YJacceleration[taskid-1], 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                break;
            }
            //========================= check if this YJ will crash ================================


            // If not crash, check if dock with Buzzy
            if(curRelaDis <= 50)
            {
                YJacceleration[taskid-1][0] = 0;
                YJacceleration[taskid-1][1] = 0;
                YJacceleration[taskid-1][2] = 0;
                
                // the current position of docked YJ is with Buzzy
                for(int ii=0; ii<3; ii++) 
                {
                    BuzzyPosition[ii] += BuzzyVelocity * BuzzyDirection[ii];
                }
                
                // send docked YJ information back to Buzzy and stop
                YJStatus[taskid - 1] = 2;
                MPI_Send(&YJStatus[taskid - 1], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Send(BuzzyPosition, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                MPI_Send(&BuzzyVelocity, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                MPI_Send(BuzzyDirection, 3, MPI_DOUBLE, 0,0, MPI_COMM_WORLD);
                MPI_Send(YJacceleration[taskid-1], 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                break;
            }

            // doesn't crush and need to adjust the plying so that YJs can dock
            /*
            Based on the hint, we can divide the process to 5 phases
            1. Each YJ adjusts itself to keep the same speed as Buzzy;
            2. Wait until every YJ finishes phase1;
            3. Pick out the closet YJ
            4. Picked YJ begin to approaching Buzzy
            5. Slow down when meeting the upper limit (as the thrusts are not stable, we hardcode as 80) 
                and stop thrusters to keep min speed (hardcode as 8)
            6. check direction
            */

            if(crash == 0 && YJStatus[taskid-1] != 2 && YJStatus[taskid-1]!=0)
            {
                // phase1: all YJs keep the same speed as Buzzy
                if(phase[taskid-1] == 0)
                { 
                    if(relaV > 0.01)
                    {
                        // ============================ adjust the velocity and direction =============================
                        if(curRelaVelo[0] >= a_max*0.3) YJacceleration[taskid-1][0] = -a_max*0.3*(rand() / double(RAND_MAX) * 0.4 + 0.8);
                        else if(curRelaVelo[0] <= -a_max) YJacceleration[taskid-1][0] = a_max*0.3*(rand() / double(RAND_MAX) * 0.4 + 0.8);
                        else YJacceleration[taskid-1][0] = (-curRelaVelo[0])*(rand() / double(RAND_MAX) * 0.4 + 0.8);

                        if(curRelaVelo[1] >= a_max*0.3) YJacceleration[taskid-1][1] = -a_max*0.3*(rand() / double(RAND_MAX) * 0.4 + 0.8);
                        else if(curRelaVelo[1] <= -a_max*0.3) YJacceleration[taskid-1][1] = a_max*0.3*(rand() / double(RAND_MAX) * 0.4 + 0.8);
                        else YJacceleration[taskid-1][1] = (-curRelaVelo[1])*(rand() / double(RAND_MAX) * 0.4 + 0.8);

                        if(curRelaVelo[2] >= a_max*0.3) YJacceleration[taskid-1][2] = -a_max*0.3*(rand() / double(RAND_MAX) * 0.4 + 0.8);
                        else if(curRelaVelo[2] <= -a_max*0.3) YJacceleration[taskid-1][2] = (a_max*0.3)*(rand() / double(RAND_MAX) * 0.4 + 0.8);
                        else YJacceleration[taskid-1][2] = (-curRelaVelo[2])*(rand() / double(RAND_MAX) * 0.4 + 0.8);
                        // ============================ adjust the velocity and direction =============================
                    }
                    else
                    {
                        // completed adjustion
                        if(signal == 1)
                        {
                            // this is the first time to go through this phase
                            phase[taskid-1] = 1;
                            wait[taskid-1] = 30;
                            signal = 0;
                        }
                        else{
                            // this reveals that this is in the process of re-adjusting direction, no need to wait in phase 2
                            phase[taskid-1] = 3;
                        }

                        
                    }
                }

                // phase2, wait for all YJs to complete adjusting
                // another way is to create nonblocking processor
                else if(phase[taskid - 1] == 1)
                {
                    if(wait[taskid-1] >0)
                    {
                        // wait 30 seconds
                        wait[taskid-1] -= 1;
                        // close all accelerators
                        YJacc[taskid-1][0] = 0;
                        YJacc[taskid-1][1] = 0;
                        YJacc[taskid-1][2] = 0;

                    }
                    else
                    {
                        // stop waiting, enter phase 3;
                        phase[taskid-1] = 2;
                    }
                }

                // phase 3, all other YJs wait until the closest one docked
                else if(phase[taskid - 1] == 2)
                {
                    // choose one closest YJ to dock, others stay in phase 3
                    int flag = 1;
                    for(int k=0; k<7; k++)
                    {
                        if(k != taskid-1 && YJStatus[k]!=0 && YJStatus[k]!=2)
                        {
                            if(curRelaDis >= pow((pow(YJPosition[k][0]-BuzzyPosition[0],2)+pow(YJPosition[k][1]-BuzzyPosition[1],2)+pow(YJPosition[k][2]-BuzzyPosition[2],2)),0.5))
                            {
                                flag = 0;
                                break;
                            }
                        }
                    }
                    if(flag == 1)
                    {
                        // enter phase 4, begin approaching and docking
                        phase[taskid-1] = 3;
                    }
                    else
                    {
                        // shut all thrusts
                        YJacceleration[taskid-1][0] = 0;
                        YJacceleration[taskid-1][1] = 0;
                        YJacceleration[taskid-1][2] = 0;
                    }
                }

                // phase 4, approaching Buzzy
                else if(phase[taskid-1] == 3)
                {
                    //double random_factor2 = rand() / double(RAND_MAX) * 0.4 + 0.8;
                    YJacceleration[taskid-1][0] = (rand() / double(RAND_MAX) * 0.4 + 0.8) * a_max * 0.5 *(-curRelaPos[0]) / curRelaDis;
                    YJacceleration[taskid-1][1] = (rand() / double(RAND_MAX) * 0.4 + 0.8) * a_max * 0.5 *(-curRelaPos[1]) / curRelaDis;
                    YJacceleration[taskid-1][2] = (rand() / double(RAND_MAX) * 0.4 + 0.8) * a_max * 0.5 *(-curRelaPos[2]) / curRelaDis;

                    if(relaV > v_max) 
                    {
                        //double random_factor3 = rand() / double(RAND_MAX) * 0.4 + 0.8;
                        phase[taskid-1] = 4;
                        YJacceleration[taskid-1][0] = (rand() / double(RAND_MAX) * 0.4 + 0.8) * a_max * 0.5 * (curRelaPos[0]) / curRelaDis;
                        YJacceleration[taskid-1][1] = (rand() / double(RAND_MAX) * 0.4 + 0.8) * a_max * 0.5 * (curRelaPos[1]) / curRelaDis;
                        YJacceleration[taskid-1][2] = (rand() / double(RAND_MAX) * 0.4 + 0.8) * a_max * 0.5 * (curRelaPos[2]) / curRelaDis;
                    }
                    

                }

                // phase 5, slown down to avoid crash, set the limitation to avoid missing Buzzy
                else if(phase[taskid-1] == 4)
                {
                    //double random_factor4 = rand() / double(RAND_MAX) * 0.4 + 0.8;
                    YJacceleration[taskid-1][0] = (rand() / double(RAND_MAX) * 0.4 + 0.8) * a_max * 0.5 * (curRelaPos[0]) / curRelaDis;
                    YJacceleration[taskid-1][1] = (rand() / double(RAND_MAX) * 0.4 + 0.8) * a_max * 0.5 * (curRelaPos[1]) / curRelaDis;
                    YJacceleration[taskid-1][2] = (rand() / double(RAND_MAX) * 0.4 + 0.8) * a_max * 0.5 *(curRelaPos[2]) / curRelaDis;

                    // reaches the bottom of V
                    if(relaV < 8)
                    {
                        phase[taskid-1] = 5;
                        // keep this speed 
                        YJacceleration[taskid-1][0] = 0;
                        YJacceleration[taskid-1][1] = 0;
                        YJacceleration[taskid-1][2] = 0;
                    }
                }
                
                // phase 6: 
                else if(phase[taskid-1] == 5)
                {
                    YJacceleration[taskid-1][0] = 0;
                    YJacceleration[taskid-1][1] = 0;
                    YJacceleration[taskid-1][2] = 0;
                }

                // check direction, cos>0.9
                double check = -(curRelaPos[0]*curRelaVelo[0] + curRelaPos[1]*curRelaVelo[1] + curRelaPos[2]*curRelaVelo[2]) / (relaV * curRelaDis);
                if(phase[taskid-1] == 5 && check<0.9)
                {
                    // wrong direction, return to phase 1 and re-adjust the direction
                    phase[taskid-1] = 0;
                    v_max = 8;
                }
            }

            // calculate the location of YJ
            for(int ii=0; ii<3; ii++)
            {
                YJPosition[taskid-1][ii] = YJPosition[taskid-1][ii] + YJVelocity[taskid-1] * YJDirection[taskid-1][ii] + YJacceleration[taskid-1][ii]/2;
            }
            
            //calculate the velocity of YJ
            double velocity_x_tmp = YJVelocity[taskid-1] * YJDirection[taskid-1][0] + YJacceleration[taskid-1][0];
            double velocity_y_tmp = YJVelocity[taskid-1] * YJDirection[taskid-1][1] + YJacceleration[taskid-1][1];
            double velocity_z_tmp = YJVelocity[taskid-1] * YJDirection[taskid-1][2] + YJacceleration[taskid-1][2];
            double velocity_tmp = pow(pow(velocity_x_tmp,2)+pow(velocity_y_tmp,2)+pow(velocity_z_tmp,2),0.5);
            YJVelocity[taskid-1] = velocity_tmp;

            // calculate the direction of YJ
            YJDirection[taskid-1][0] = velocity_x_tmp/velocity_tmp;
            YJDirection[taskid-1][1] = velocity_y_tmp/velocity_tmp;
            YJDirection[taskid-1][2] = velocity_z_tmp/velocity_tmp;

            // send back to Buzzy
            MPI_Send(&YJStatus[taskid-1], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(YJPosition[taskid-1], 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            MPI_Send(&YJVelocity[taskid-1], 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            MPI_Send(YJDirection[taskid-1], 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            MPI_Send(YJacceleration[taskid-1], 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize(); //shuts everything down

}

