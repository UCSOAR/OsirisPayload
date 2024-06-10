/**
 ******************************************************************************
 * File Name          : main_system.cpp
 * Description        : This file acts as an interface supporting CubeIDE Codegen
    while having a clean interface for development.
 ******************************************************************************
*/
/* Includes -----------------------------------------------------------------*/
#include "SystemDefines.hpp"
#include "UARTDriver.hpp"

// Tasks
#include "CubeTask.hpp"
#include "DebugTask.hpp"
#include "FlightTask.hpp"
#include "IMUTask//Inc//IMUTask.hpp"

/* Drivers ------------------------------------------------------------------*/
namespace Driver {
    UARTDriver uart6(USART6);
}

/* Interface Functions ------------------------------------------------------------*/
/**
 * @brief Main function interface, called inside main.cpp before os initialization takes place.
*/
void run_main() {
    // Init Tasks
    CubeTask::Inst().InitTask();
    DebugTask::Inst().InitTask();
    FlightTask::Inst().InitTask();
    IMUTask::Inst().InitTask();

    // Print System Boot Info : Warning, don't queue more than 10 prints before scheduler starts
    SOAR_PRINT("\n-- SOAR SYSTEM --\n");
    SOAR_PRINT("System Reset Reason: [TODO]\n"); //TODO: System reset reason can be implemented via. Flash storage
    SOAR_PRINT("Current System Free Heap: %d Bytes\n", xPortGetFreeHeapSize());
    SOAR_PRINT("Lowest Ever Free Heap: %d Bytes\n\n", xPortGetMinimumEverFreeHeapSize());

    Command testIMU(REQUEST_COMMAND, IMU_REQUEST_LIN_ACC);
	IMUTask::Inst().GetEventQueue()->Send(testIMU);

    // Start the Scheduler
    // Guidelines:
    // - Be CAREFUL with race conditions after osKernelStart
    // - All uses of new and delete should be closely monitored after this point
    osKernelStart();

    // Should never reach here
    SOAR_ASSERT(false, "osKernelStart() failed");

    while (1)
    {
        osDelay(100);
        HAL_NVIC_SystemReset();
    }
}
