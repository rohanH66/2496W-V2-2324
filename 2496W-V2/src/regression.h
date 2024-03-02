#ifndef __REG__
#define __REG__

#include "main.h"
#include "global.h"
#include "driver.h"
#include "auton.h"
#include "auton_func.h"
#include "pid.h"
#include "lib/auton_obj.h"
#include "lib/chassis.h"
#include "lib/piston.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <string>
#include <array>
#include <stdio.h>
#include <cstring>


using namespace glb;
using namespace pros;


namespace reg {
    class regression {
    public:
        std::vector<double> xData;
        std::vector<double> yData;
        double mean(const vector<double>& data) const {
            return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
        }

        int getSize(){
            return xData.size();
        }
        double slope() const {
            double numerator = 0.0;
            double denominator = 0.0;
            double xMean = mean(xData);
            double yMean = mean(yData);
            for (int i = 0; i < xData.size(); ++i) {
                numerator += (xData[i] - xMean) * (yData[i] - yMean);
                denominator += (xData[i] - xMean) * (xData[i] - xMean);
            }
            return numerator / denominator;
        }
        double intercept() const {
            return mean(yData) - slope() * mean(xData);
        }
        void addData(double x, double y) {
            xData.push_back(x);
            yData.push_back(y);
        }
        double getXIntercept() const {
            double m = slope();
            if (m == 0) {
                return 0.0;
            }
            double b = intercept();
            return -b / m;
        }
        void printCSV() const {
            printf("X,Y\n");
            for (int i = 0; i < xData.size(); ++i) {
                std::cout << xData[i] << "," << yData[i] << "\n";
            }
        }
        double predictNext(double x) const {
            return intercept() + slope() * x;
        }
        double correlationCoefficient() const {
            double numerator = 0.0;
            double xMean = mean(xData);
            double yMean = mean(yData);
            double sxx = 0.0;
            double syy = 0.0;
            for (size_t i = 0; i < xData.size(); ++i) {
                numerator += (xData[i] - xMean) * (yData[i] - yMean);
                sxx += (xData[i] - xMean) * (xData[i] - xMean);
                syy += (yData[i] - yMean) * (yData[i] - yMean);
            }
            return numerator / sqrt(sxx * syy);
        }
        double rSquared() const {
            return correlationCoefficient() * correlationCoefficient();
        }
    };


    int get_reg(regression re, int init){
        return (re.getSize() > 2) ? re.getXIntercept() : (init + re.getSize() * 0.1);
    }

    std::array<std::string, 3> getResults(regression re) {
        std::array<std::string, 3> results;
        
        // Coefficients
        double m = re.slope();
        double b = re.intercept();
        results[0] = "slope " + std::to_string(m) + ", intercept " + std::to_string(b);
        
        // Dataset size
        results[1] = "K val : " + std::to_string(re.getXIntercept()) +"w/ size"+ std::to_string(re.getSize());
        
        // Correlation coefficient and R squared
        double r = re.correlationCoefficient();
        double rSquaredValue = r * r;
        results[2] = "r: " + std::to_string(r) + ", R^2: " + std::to_string(rSquaredValue);
        
        return results;
    }



    // main regression model runner (super cool! poggers)
    void regMain(int target, int init_trials, double guess){
        //glb::con.clear();
        const int iterations = 6;
        delay(100);
        printf("kP, error\n");
        delay(100);
        regression r;
        bool cont = true;
        bool neg = false;
        double nextK;
        double error;


        while (cont){
            delay(100);
            glb::con.clear();
            delay(60);
            con.print(0,0,"Reg K=%.4f" , target);
            
            error = pid::driveR(target, guess);
            r.addData(guess, error);
            pid::driveR(-target, guess);
            delay(50);
            printf("%.4f, %.4f\n", guess, error);
            delay(50);

            error = pid::driveR(target, guess+0.01);
            r.addData(guess+0.01, error);
            pid::driveR(-target, guess+0.01);
            delay(50);
            printf("%.4f, %.4f\n", guess+0.01, error);
            delay(50);
            
            int count = 2;
            nextK = r.getXIntercept();
            for (int i=0;i<init_trials;i++){
                for (int j=0;j<iterations;j++){
                    error = pid::driveR(target, nextK);
                    count++;
                    r.addData(nextK, error);
                    pid::driveR(-target, nextK);
                    delay(100);
                    printf("%.4f, %.4f\n", nextK, error);
                    delay(100);
                }
                nextK = r.getXIntercept();
            }

            std::array<std::string, 3> results = getResults(r);
            string one = results[0];
            string two = results[1];
            string three = results[2];
            delay(50);

            con.clear();
            delay(50);
            con.print(0,0,"%s                ",one);
            delay(50);
            con.print(1,0,"%s                ",two);
            delay(50);
            con.print(2,0,"%s                ",three);
            delay(50);
            printf("%s                ",one);
            delay(50);
            printf("%s                ",two);
            delay(50);
            printf("%s                ",three);
            delay(50);
            while (true){
                if (con.get_digital_new_press(E_CONTROLLER_DIGITAL_A)){
                    init_trials = 5;
                    cont = true; 
                    break;
                }
                if (con.get_digital_new_press(E_CONTROLLER_DIGITAL_B)){ 
                    cont = false;
                    break; 
                }
                delay(5);
            }
        }
    }
}




#endif