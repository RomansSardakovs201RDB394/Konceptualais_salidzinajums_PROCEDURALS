#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <windows.h> 
#include <Psapi.h>
#include <thread>

size_t allocatedBytes = 0;
void* operator new(size_t size) {
    allocatedBytes = allocatedBytes + size;
    return malloc(size);
}

size_t AverageRAM = 0;
size_t MaxRAM = 0;
int RAMcounter = 0;
HANDLE process = GetCurrentProcess();
void RAMMONITOR() {
    while (true) {
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(process, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        SIZE_T physMemUsedByMe = pmc.PrivateUsage;
        MaxRAM = pmc.PeakWorkingSetSize;
        AverageRAM = AverageRAM + physMemUsedByMe;
        RAMcounter++;
    }
}

int CPUcounter = 0;
double AverageCPU = 0.0;
void CPUMONITOR() {
    FILETIME idleTime, kernelTime, userTime;
    ULARGE_INTEGER lastIdleTime{}, lastKernelTime{}, lastUserTime{};
    double cpuUsage = 0.0;

    while (true) {
        GetSystemTimes(&idleTime, &kernelTime, &userTime);
        ULARGE_INTEGER currentIdleTime = { idleTime.dwLowDateTime, idleTime.dwHighDateTime };
        ULARGE_INTEGER currentKernelTime = { kernelTime.dwLowDateTime, kernelTime.dwHighDateTime };
        ULARGE_INTEGER currentUserTime = { userTime.dwLowDateTime, userTime.dwHighDateTime };

        if (lastIdleTime.QuadPart != 0) {
            ULONGLONG idleDiff = currentIdleTime.QuadPart - lastIdleTime.QuadPart;
            ULONGLONG kernelDiff = currentKernelTime.QuadPart - lastKernelTime.QuadPart;
            ULONGLONG userDiff = currentUserTime.QuadPart - lastUserTime.QuadPart;

            ULONGLONG totalDiff = kernelDiff + userDiff;
            cpuUsage = (1.0 - (double)idleDiff / (double)totalDiff) * 100.0;
        }

        lastIdleTime = currentIdleTime;
        lastKernelTime = currentKernelTime;
        lastUserTime = currentUserTime;

        AverageCPU = AverageCPU + cpuUsage;
        CPUcounter++;
        Sleep(1000);
    }
}

void CalculateAverageCPU() {
    std::cout << "AVERAGE CPU USAGE: " << (AverageCPU / CPUcounter) << '%' << std::endl;
}

void CalculateAverageRAM() {
    std::cout << "AVERAGE RAM USAGE: " << (AverageRAM / RAMcounter) << " bytes" << std::endl;
}


std::vector<struct VehicleInstance> VehicleList;

struct VehicleInstance {

    std::string objectName = "NaN";

    std::string type = "NaN";

    std::string name = "NaN";

    std::map<short, short> engine;

    std::string color = "NaN";

    short wheels = 0;
    float battery = 0.0;
    short mass = 0;
    short breaks = 0;

    short max_speed = 0;

    struct {

        std::string body = "NaN";

        std::vector<std::string> electro_systems;

    } CarInstance;

};
typedef struct VehicleInstance vehicle;

void CreateVehicle(std::string ThisobjectName, std::string ThisName, short NrEngine, short power, std::string Thiscolor, short Thiswheels, float Thisbattery, short Thismass, short Thisbreaks, short ThisMaxSpeed) {

    vehicle ThisVehicle;

    ThisVehicle.objectName = ThisobjectName;
    ThisVehicle.name = ThisName;
    ThisVehicle.engine.insert(std::make_pair(NrEngine, power));
    ThisVehicle.color = Thiscolor;
    ThisVehicle.wheels = Thiswheels;
    ThisVehicle.battery = Thisbattery;
    ThisVehicle.mass = Thismass;
    ThisVehicle.breaks = Thisbreaks;
    ThisVehicle.max_speed = ThisMaxSpeed;

    ThisVehicle.type = "Vehicle";

    VehicleList.push_back(ThisVehicle);

}

void CreateVehicleCar(std::string ThisobjectName, std::string ThisName, short NrEngine, short power, std::string Thiscolor, short Thiswheels, float Thisbattery, short Thismass, short Thisbreaks, short ThisMaxSpeed, std::string ThisBody, std::vector<std::string> ThisElectroSystems) {

    vehicle ThisVehicle;

    ThisVehicle.objectName = ThisobjectName;
    ThisVehicle.name = ThisName;
    ThisVehicle.engine.insert(std::make_pair(NrEngine, power));
    ThisVehicle.color = Thiscolor;
    ThisVehicle.wheels = Thiswheels;
    ThisVehicle.battery = Thisbattery;
    ThisVehicle.mass = Thismass;
    ThisVehicle.breaks = Thisbreaks;
    ThisVehicle.max_speed = ThisMaxSpeed;

    ThisVehicle.CarInstance.body = ThisBody;
    ThisVehicle.CarInstance.electro_systems = ThisElectroSystems;

    ThisVehicle.type = "Car";

    VehicleList.push_back(ThisVehicle);

}

struct VehicleInstance FindVehicle(std::string objectName) {
    vehicle ThisVehicle;
    for (int i = 0; i < VehicleList.size(); i++) {
        if (VehicleList.at(i).objectName == objectName) {
            return VehicleList.at(i);
        }
    }
    std::cout << "Vehicle with object name: " << objectName << " not found" << std::endl;
    return ThisVehicle;
}

void accelerate(short NrEngine, int seconds, std::string objectName) {
    vehicle ThisVehicle;
    ThisVehicle = FindVehicle(objectName);
    if (ThisVehicle.objectName != objectName) { return; }
    auto accelerateSpeed = ThisVehicle.engine.find(NrEngine)->second * seconds;
    if (accelerateSpeed < ThisVehicle.max_speed) {
        std::cout << "Vehicle " << ThisVehicle.name << " acceleration of " << std::to_string(seconds) << " seconds ";
        std::cout << "reached speed of " << std::to_string(accelerateSpeed / 10) << "km/h" << std::endl;
    }
    else {
        std::cout << "Vehicle " << ThisVehicle.name << " acceleration of " << std::to_string(seconds) << " seconds ";
        std::cout << "reached max speed of " << std::to_string(ThisVehicle.max_speed) << "km/h" << std::endl;
    }
}

void reachTopSpeed(std::string objectName) {
    vehicle ThisVehicle;
    ThisVehicle = FindVehicle(objectName);
    if (ThisVehicle.objectName != objectName) { return; }
    std::cout << "Vehicle " << ThisVehicle.name << " reached top speed " << std::to_string(ThisVehicle.max_speed) << "km/h" << std::endl;
}

void useBreaks(std::string objectName) {
    vehicle ThisVehicle;
    ThisVehicle = FindVehicle(objectName);
    if (ThisVehicle.objectName != objectName) { return; }
    std::cout << "vehicle " << ThisVehicle.name << " use breaks" << std::endl;
}

void startEngine(std::string objectName) {
    vehicle ThisVehicle;
    ThisVehicle = FindVehicle(objectName);
    if (ThisVehicle.objectName != objectName) { return; }
    if (ThisVehicle.engine.empty()) {
        std::cout << "Vehicle " << ThisVehicle.name << " do not have an engine" << std::endl;
        return;
    }
    std::cout << "Vehicle " << ThisVehicle.name << " Engine started" << std::endl;
}

void stopEngine(std::string objectName) {
    vehicle ThisVehicle;
    ThisVehicle = FindVehicle(objectName);
    if (ThisVehicle.objectName != objectName) { return; }
    if (ThisVehicle.engine.empty()) {
        std::cout << "Vehicle " << ThisVehicle.name << " do not have an engine" << std::endl;
        return;
    }
    std::cout << "Vehicle " << ThisVehicle.name << " engine stopped" << std::endl;
}

void CheckElectroSystems(std::string objectName) {
    vehicle ThisVehicle;
    ThisVehicle = FindVehicle(objectName);
    if (ThisVehicle.objectName != objectName) { return; }
    if (ThisVehicle.CarInstance.electro_systems.empty()) {
        std::cout << "Vehicle " << ThisVehicle.name << " has no electro systems!" << std::endl;
        return;
    }
    for (std::string& val : ThisVehicle.CarInstance.electro_systems) {
        std::cout << "Checking vehicle " << ThisVehicle.name << " " << val << "..." << std::endl;
        Sleep(500);
    }
    std::cout << "Vehicle " << ThisVehicle.name << " all systems allright" << std::endl;
}

void registrateVehicle(std::string objectName) {
    vehicle ThisVehicle;
    ThisVehicle = FindVehicle(objectName);

    if (ThisVehicle.type != "Car") {
        std::cout << "Vehicle " << ThisVehicle.name << " was registred as Vehicle type" << std::endl;
        return;
    }
    std::cout << "Vehicle " << ThisVehicle.name << " was registred as Car type" << std::endl;
}

int main() {
    std::thread CpuMonitorThread(CPUMONITOR);
    std::thread ramMonitorThread(RAMMONITOR);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::string> audiElectroSystems = { "ABS","ESP" };

    CreateVehicleCar("firstAudi", "audi a6", 1, 220, "Black", 4, 13.2, 1248, 2000, 260, "sedan", audiElectroSystems);

    CheckElectroSystems("firstAudi");
    startEngine("firstAudi");
    accelerate(1, 10, "firstAudi");
    registrateVehicle("firstAudi");

    std::cout << std::endl;

    CreateVehicle("firstMyCar", "My car", 1, 80, "green", 4, 8.2, 420, 860, 80);

    startEngine("firstMyCar");
    accelerate(1, 10, "firstMyCar");
    registrateVehicle("firstMyCar");

    std::cout << "----------RESULTS----------" << std::endl;
    CalculateAverageRAM();
    CalculateAverageCPU();
    std::cout << "All Time ALLOCATED: " << allocatedBytes << " bytes" << std::endl;
    std::cout << "PEAK RAM USAGE: " << MaxRAM << " bytes" << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> endClock = end - start;
    std::cout << "Execution time: " << endClock.count() << " seconds";
    std::cout << std::endl;

    CloseHandle(process);
    exit(-1);
    return 0;
}
