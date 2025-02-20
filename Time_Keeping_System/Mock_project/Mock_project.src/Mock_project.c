#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Mock_project.h"

#define ADMIN_PASSWORD "12345678"
#define MAX_EMPLOYEES 100
#define EMPLOYEE_FILE "employees.dat"
#define WORKING_TIME_FILE "working_time.dat"
#define SALARY_FILE_TEMPLATE "salary_month_%d.dat"

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


Employee employees[MAX_EMPLOYEES];
int employee_count = 0;
char admin_password[20];
WorkingTime working_times[MAX_EMPLOYEES * 31];
int working_time_count = 0;


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


// Main Function
int main() 
{
    strcpy(admin_password, ADMIN_PASSWORD);
    loadEmployees();
    loadWorkingTime();
    createSystemInfoFile();

    int choice;
    while (1) 
    {
        printf("\n\n\n");
        printf("***********************************************\n");
        printf("*           TIME KEEPING SYSTEM               *\n");
        printf("***********************************************\n");
        printf("Main Menu:\n");
        printf("<<Enter the corresponding number to select the desired mode>>\n");
        printf("1. Admin mode\n");
        printf("2. Normal mode\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                adminMenu();
                break;
            case 2:
                normalMenu();
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}


// AdminMenu Function
void adminMenu() 
{
    char password[20];
    printf("Enter admin password: ");
    scanf("%s", password);

    if (strcmp(password, admin_password) != 0) 
    {
        printf("Incorrect password.\n");
        return;
    }

    int choice;
    while (1) 
    {
        printf("\n\n\n");
        printf("***********************************************\n");
        printf("*                  ADMIN MODE                 *\n");
        printf("***********************************************\n");
        printf("<<Enter the corresponding number to select the desired mode>>\n");
        printf("1. Change admin password\n");
        printf("2. Add employee\n");
        printf("3. View all employees\n");
        printf("4. View working time of an employee\n");
        printf("5. Calculate salary for all employees\n");
        printf("6. Calculate individual employee salary\n");
        printf("7. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                changeAdminPassword();
                break;
            case 2:
                addEmployee();
                break;
            case 3:
                viewEmployees();
                break;
            case 4:
                viewEmployeeWorkingTime();
                break;
            case 5:
                calculateSalary();
                break;
            case 6:
                calculateIndividualSalary();
                break;
            case 7:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}


void normalMenu() 
{
    int choice;
    while (1) 
    {
        printf("\n\n\n");
        printf("***********************************************\n");
        printf("*                  NORMAL MODE                *\n");
        printf("***********************************************\n");
        printf("<<Enter the corresponding number to select the desired mode>>\n");
        printf("1. Check-in\n");
        printf("2. Check-out\n");
        printf("3. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                checkIn();
                break;
            case 2:
                checkOut();
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}


void changeAdminPassword() 
{
    char newPassword[20];
    printf("\nEnter new password: ");
    scanf("%s", newPassword);
    strcpy(admin_password, newPassword);
    printf("Password changed successfully.\n");
}


void addEmployee() 
{
    if (employee_count >= MAX_EMPLOYEES) 
    {
        printf("Employee limit reached.\n");
        return;
    }

    Employee emp;
    emp.id = employee_count + 1;
    printf("\nEnter full name: ");
    scanf(" %[^\n]", emp.name);
    printf("Enter basic salary (VND/hour): ");
    scanf("%f", &emp.basic_salary);

    employees[employee_count] = emp;
    employee_count++;

    saveEmployees();
    printf("=> Employee added successfully with ID: %d\n", emp.id);
}



void viewEmployees() 
{
    for (int i = 0; i < employee_count; i++) 
    {
        printf("ID: %d, Name: %s, Basic Salary: %.f VND/hour\n",
               employees[i].id, employees[i].name, employees[i].basic_salary);
    }
}



void viewEmployeeWorkingTime() 
{
    int empId;
    printf("\nEnter employee ID: ");
    scanf("%d", &empId);

    for (int i = 0; i < working_time_count; i++) 
    {
        if (working_times[i].employee_id == empId) 
        {
            printf("Check-in: %02d-%02d-%04d %02d:%02d\n",
                   working_times[i].check_in.tm_mday,
                   working_times[i].check_in.tm_mon + 1,
                   working_times[i].check_in.tm_year + 1900,
                   working_times[i].check_in.tm_hour,
                   working_times[i].check_in.tm_min);

            printf("Check-out: %02d-%02d-%04d %02d:%02d\n",
                   working_times[i].check_out.tm_mday,
                   working_times[i].check_out.tm_mon + 1,
                   working_times[i].check_out.tm_year + 1900,
                   working_times[i].check_out.tm_hour,
                   working_times[i].check_out.tm_min);
        }
    }
}



void calculateSalary() 
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int month = tm_info->tm_mon + 1;

    for (int i = 0; i < employee_count; i++) 
    {
        int empId = employees[i].id;
        float total_salary = 0;
        float total_hours = 0;
        int total_penalty = 0;

        for (int j = 0; j < working_time_count; j++) 
        {
            if (working_times[j].employee_id == empId) 
            {
                float hours = calculateWorkingHours(working_times[j]);
                total_hours += hours;
                total_penalty += calculatePenalty(working_times[j]);
            }
        }

        if (total_hours > 184) 
        {
            total_hours = 184;
        }

        total_salary = total_hours * employees[i].basic_salary - total_penalty;
        printf("ID: %d, Name: %s, Total Salary: %.f VND\n",
               employees[i].id, employees[i].name, total_salary);
    }

    saveSalary(month);
    deletePreviousSalaryFile(month - 1);
}



void calculateIndividualSalary() 
{
    int empId;
    printf("Enter employee ID: ");
    scanf("%d", &empId);

    int working_days = getWorkingDays(empId);
    if (working_days < 23) 
    {
        printf("This employee has only worked %d days.\n", working_days);
        printf("Do you want to view this employee's salary? (y/n): ");
    } else {
        printf("This employee has worked more than 23 days.\n");
        printf("Do you want to calculate salary after 31 days? (y/n): ");
    }

    char choice;
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') 
    {
        float total_salary = 0;
        float total_hours = 0;
        int total_penalty = 0;

        for (int i = 0; i < working_time_count; i++) 
        {
            if (working_times[i].employee_id == empId) 
            {
                float hours = calculateWorkingHours(working_times[i]);
                total_hours += hours;
                total_penalty += calculatePenalty(working_times[i]);
            }
        }

        if (total_hours > 184) 
        {
            total_hours = 184;
        }

        total_salary = total_hours * employees[empId - 1].basic_salary - total_penalty;
        printf("ID: %d, Name: %s, Total Salary: %.f VND\n",
               employees[empId - 1].id, employees[empId - 1].name, total_salary);
    }
}



void saveEmployees() 
{
    FILE *file = fopen(EMPLOYEE_FILE, "wb");
    if (!file) {
        perror("Error opening file for saving employees");
        return;
    }
    fwrite(&employee_count, sizeof(int), 1, file);
    fwrite(employees, sizeof(Employee), employee_count, file);
    fclose(file);
}



void loadEmployees() 
{
    FILE *file = fopen(EMPLOYEE_FILE, "rb");
    if (file) 
    {
        fread(&employee_count, sizeof(int), 1, file);
        fread(employees, sizeof(Employee), employee_count, file);
        fclose(file);
    } else 
    {
        employee_count = 0;
    }
}



void checkIn() 
{
    int empId;
    printf("Enter employee ID: ");
    scanf("%d", &empId);

    for (int i = 0; i < employee_count; i++) 
    {
        if (employees[i].id == empId) 
        {
            printf("Employee: %s\n", employees[i].name);
            char confirm;
            printf("Confirm check-in? (y/n): ");
            scanf(" %c", &confirm);
            if (confirm == 'y' || confirm == 'Y') 
            {
                time_t t = time(NULL);
                struct tm check_in = *localtime(&t);
                WorkingTime wt = { empId, check_in, { 0 } };
                working_times[working_time_count++] = wt;
                saveWorkingTime(&wt);
                printf("Check-in time saved.\n");
                return;
            }
            return;
        }
    }
    printf("Invalid employee ID.\n");
}



void checkOut() 
{
    int empId;
    printf("Enter employee ID: ");
    scanf("%d", &empId);

    for (int i = 0; i < employee_count; i++) 
    {
        if (employees[i].id == empId) 
        {
            printf("Employee: %s\n", employees[i].name);
            char confirm;
            printf("Confirm check-out? (y/n): ");
            scanf(" %c", &confirm);
            if (confirm == 'y' || confirm == 'Y') 
            {
                time_t t = time(NULL);
                struct tm check_out = *localtime(&t);
                for (int j = 0; j < working_time_count; j++) 
                {
                    if (working_times[j].employee_id == empId &&
                        working_times[j].check_out.tm_year == 0) 
                        {
                        working_times[j].check_out = check_out;
                        saveWorkingTime(&working_times[j]);
                        printf("Check-out time saved.\n");
                        return;
                    }
                }
                printf("No check-in record found for today.\n");
            }
            return;
        }
    }
    printf("Invalid employee ID.\n");
}



void saveWorkingTime(WorkingTime *wt) 
{
    FILE *file = fopen(WORKING_TIME_FILE, "ab");
    if (!file) 
    {
        perror("Error opening file for saving working times");
        return;
    }
    fwrite(wt, sizeof(WorkingTime), 1, file);
    fclose(file);
}



void loadWorkingTime() 
{
    FILE *file = fopen(WORKING_TIME_FILE, "rb");
    if (file) 
    {
        while (fread(&working_times[working_time_count], sizeof(WorkingTime), 1, file)) 
        {
            working_time_count++;
        }
        fclose(file);
    }
}



void saveSalary(int month) 
{
    char filename[25];
    sprintf(filename, SALARY_FILE_TEMPLATE, month);
    FILE *file = fopen(filename, "wb");
    if (!file) 
    {
        perror("Error opening file for saving salary");
        return;
    }
    // Assuming some salary data structure to save
    // fwrite(salary_data, sizeof(SalaryData), number_of_employees, file);
    fclose(file);
}



void deletePreviousSalaryFile(int month) 
{
    if (month < 1) month += 12;
    char filename[50];
    sprintf(filename, SALARY_FILE_TEMPLATE, month);
    remove(filename);
}



void createSystemInfoFile() 
{
    FILE *file = fopen("system_info.txt", "w");
    if (!file) 
    {
        perror("Error opening file for saving system info");
        return;
    }
    fprintf(file, "Default password: %s\n", ADMIN_PASSWORD);
    fprintf(file, "The working day starts at 8am and ends at 5pm.\n");
    fprintf(file, "Admin enters the basic salary of the employee in VND, do not enter the \".\".\n");
    fprintf(file, "Calculate salary periodically on the 15th of each month.\n");
    fclose(file);
}



int getWorkingDays(int empId) 
{
    int count = 0;
    for (int i = 0; i < working_time_count; i++) 
    {
        if (working_times[i].employee_id == empId) 
        {
            count++;
        }
    }
    return count;
}



float calculateWorkingHours(WorkingTime wt) 
{
    if (wt.check_in.tm_year == 0 || wt.check_out.tm_year == 0) 
    {
        return 0;
    }

    int start_hour = wt.check_in.tm_hour;
    int start_min = wt.check_in.tm_min;
    int end_hour = wt.check_out.tm_hour;
    int end_min = wt.check_out.tm_min;

    // Ensure valid working hours
    if (start_hour < 8 || (start_hour == 8 && start_min < 0)) 
    {
        start_hour = 8;
        start_min = 0;
    }
    
    if (end_hour > 17 || (end_hour == 17 && end_min > 0)) 
    {
        end_hour = 17;
        end_min = 0;
    }

    if ( start_hour > 12 || (start_hour == 12 && start_min > 0 ))
    {
        start_hour = 13;
        start_min = 0;
    }

    if ( end_hour < 13 || (end_hour == 13 && end_min < 0)) 
    {
        end_hour = 12;
        end_min = 0;
    }

    // Adjust for lunch break
    if (start_hour < 12 && end_hour > 13) 
    {
        end_hour -= 1; // Subtract lunch break
    }
    

    // Convert times to float for easier calculation
    float start_time = start_hour + start_min / 60.0;
    float end_time = end_hour + end_min / 60.0;
    float hours = end_time - start_time;

    if (hours < 0) 
    {
        hours = 0;
    }

    return hours;
}



float calculatePenalty(WorkingTime wt) 
{
    int penalty = 0;
    if ( 12 > wt.check_in.tm_hour > 8 || (wt.check_in.tm_hour == 8 && wt.check_in.tm_min > 30)) 
    {
        penalty += 20000;
    }
    if ( 13 < wt.check_out.tm_hour < 16 ||(wt.check_out.tm_hour == 16 && wt.check_out.tm_min < 30)) 
    {
        penalty += 20000;
    }
    return penalty;
}


