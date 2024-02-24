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


using namespace glb;
using namespace std;
using namespace pros;

namespace reg {
    class regression {
    public:
        std::vector<double> xData;
        std::vector<double> yData;
        double mean(const std::vector<double>& data) const {
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
            for (size_t i = 0; i < xData.size(); ++i) {
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
                return std::numeric_limits<double>::quiet_NaN(); // Return NaN to indicate an undefined result
            }
            double b = intercept();
            return -b / m;
        }
        void printCSV() const {
            std::cout << "X,Y\n";
            for (size_t i = 0; i < xData.size(); ++i) {
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
    void regMain(int target){
        glb::con.clear();
        regression r;
        bool cont = true;
        bool neg = false;
        double nextK;
        double error;

        while (cont){
            glb::con.

            nextK = r.getXIntercept();
            error = pid::driveR(target, nextK);
            r.addData(nextK, error);

        }


    }



}




#endif