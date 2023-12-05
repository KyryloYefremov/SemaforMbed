#include "headers.h"
Thread car1; 
Thread car2; 
Thread car3; 
Thread car4; 
Thread car5; 
Thread car6; 
Thread car7; 
Semaphore parking_semaphore(LOTS_COUNT); 
Mutex semaphore_mut; 
std::stack<int> park_lots; 
 
struct CarParams { 
    uint32_t color; 
    uint32_t parking_time; 
    uint32_t riding_time; 
}; 
 
 
void display(uint32_t color, int place){ 
    int x; 
    int y; 
    switch (place){ 
        case 1: 
        x = 100; 
        y = 60; 
        break; 
        case 2: 
        x = 300; 
        y = 60; 
        break; 
        case 3: 
        x = 100; 
        y = 180; 
        break; 
        case 4: 
        x = 300; 
        y = 180; 
        break; 
    } 
    BSP_LCD_SetTextColor(color); 
    BSP_LCD_DisplayStringAt(x, y, (uint8_t *)"CAR", LEFT_MODE); 
} 
 
 
void park(uint32_t time, uint32_t car_color, int lot_number)  
{ 
    // rozsviti displey na parkovacim miste 
    semaphore_mut.lock(); 
    display(car_color, lot_number); 
    semaphore_mut.unlock(); 
 
    ThisThread::sleep_for(time);     
} 
 
void leave_parking(uint32_t time, int lot_num) 
{ 
    parking_semaphore.release(); 
    // cas, ktery auto jezdi venku 
    park_lots.push(lot_num);
    display(LCD_COLOR_WHITE, lot_num);
    ThisThread::sleep_for(time); 
} 
 
 
void car(CarParams *params) 
{ 
    int lot_number; 
    while (true) 
    { 
        parking_semaphore.acquire(); 
        semaphore_mut.lock();
        lot_number = park_lots.top(); 
        park_lots.pop(); 
        semaphore_mut.unlock(); 
        park(params -> parking_time, params->color, lot_number); 
        leave_parking(params->riding_time, lot_number); 
    } 
} 
 
 
int main() 
{   
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

    for (int i = 1; i <= LOTS_COUNT; i++) 
    { 
        park_lots.push(i); 
    } 
 
    CarParams params1, params2, params3, params4, params5, params6, params7; 
    params1.color = LCD_COLOR_RED; 
    params2.color = LCD_COLOR_BLUE; 
    params3.color = LCD_COLOR_YELLOW; 
    params4.color = LCD_COLOR_GREEN; 
    params5.color = LCD_COLOR_BLACK; 
    params6.color = LCD_COLOR_ORANGE; 
    params7.color = LCD_COLOR_LIGHTCYAN; 
 
    params1.parking_time = 10000; 
    params2.parking_time = 15000; 
    params3.parking_time = 12500; 
    params4.parking_time = 5000; 
    params5.parking_time = 7000; 
    params6.parking_time = 3000; 
    params7.parking_time = 5000; 
 
    params1.riding_time = 5000; 
    params2.riding_time = 7500; 
    params3.riding_time = 2500; 
    params4.riding_time = 4000; 
    params5.riding_time = 5000; 
    params6.riding_time = 2500; 
    params7.riding_time = 3500; 
 
    car1.start(callback(car, &params1)); 
    car2.start(callback(car, &params2)); 
    car3.start(callback(car, &params3)); 
    car4.start(callback(car, &params4)); 
    car5.start(callback(car, &params5)); 
    car6.start(callback(car, &params6)); 
    car7.start(callback(car, &params7));
     
     
 
    while (true); 
}


