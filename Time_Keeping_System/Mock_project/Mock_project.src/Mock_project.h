// Guards
#ifndef FILE_H
#define FILE_H


// Standard Library Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



// Macros
#define ADMIN_PASSWORD "12345678"
#define MAX_EMPLOYEES 100
#define EMPLOYEE_FILE "employees.dat"
#define WORKING_TIME_FILE "working_time.dat"
#define SALARY_FILE_TEMPLATE "salary_month_%d.dat"



// function prototypes
void adminMenu();
void normalMenu();
void changeAdminPassword();
void addEmployee();
void viewEmployees();
void viewEmployeeWorkingTime();
void calculateSalary();
void calculateIndividualSalary();
void checkIn();
void checkOut();
void saveEmployees();
void loadEmployees();
void saveWorkingTime(WorkingTime *wt);
void loadWorkingTime();
void saveSalary(int month);
void deletePreviousSalaryFile(int month);
void createSystemInfoFile();
int getWorkingDays(int empId);
float calculateWorkingHours(WorkingTime wt);
float calculatePenalty(WorkingTime wt);



// Type Definitions
typedef struct 
{
    int id;
    char name[25];
    float basic_salary;
} Employee;

typedef struct 
{
    int employee_id;
    struct tm check_in;
    struct tm check_out;
} WorkingTime;


#endif /* FILE_H */
