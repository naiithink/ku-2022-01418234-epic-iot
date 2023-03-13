int count;

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup()
{
    Serial.begin(115200);

    Serial.printf("Setup is running on core %d\n", xPortGetCoreID());

    xTaskCreatePinnedToCore(
        task1,
        "Task1",
        2048,
        NULL,
        1,
        &Task1,
        0
    );

    xTaskCreatePinnedToCore(
        task2,
        "Task2",
        2048,
        NULL,
        1,
        &Task2,
        0
    );
}

void loop()
{
    if (count++ < 10)
        Serial.printf("Loop is running on core %d\n", xPortGetCoreID());
    else
    {
        vTaskDelete(Task1);
        vTaskDelete(Task2);
        while (true);
    }
}

void task1(void *param)
{
    for (;;)
    {
        Serial.printf("Task1 is running on core %d\n", xPortGetCoreID());
    }
}

void task2(void *param)
{
    for (;;)
    {
        Serial.printf("Task2 is running on core %d\n", xPortGetCoreID());
    }
}
