#include <mega128a.h>
#include <delay.h>
#include <stdio.h>
#include <string.h>
#include <twi.h>
#include <alcd_twi.h>


// 모듈 핀 위치
#define INTERRUPT_BUTTON_PIN PINE
#define BUTTON_PIN           PINF
#define MOTOR_PIN            PORTB
#define BUZZER_PIN           PORTG

// 입력된 버튼 
#define NO_INPUT            0b11111111 // 아무것도 안 눌린 상태
#define MODE_BUTTON         0b11111110  
#define INCREASE_BUTTON     0b11111101  
#define START_PAUSE_BUTTON  0b11111011 
#define NEXT_BUTTON         0b11110111 
#define BUTTON_4            0b11101111 
#define BUTTON_5            0b11011111
#define BUTTON_6            0b10111111
#define BUTTON_7            0b01111111 

// 모터 방향
#define RIGHT 0b00000010   // 정방향 회전
#define LEFT  0b00000001   // 역방향 회전
#define STOP  0b00000011        // 정지 

// 부저 ON/OFF
#define BUZZER_ON  0x01
#define BUZZER_OFF 0x00


// 세탁기 전원
#define OFF 0 
#define ON 1

// 세탁기 상태
#define MODE_SELECT 50
#define WASHING 51
#define RINSING 52
#define DRYING 53
#define PAUSE 54
#define END 55

// 모드 및 메뉴 인덱스 초기값
#define MODE_I_DEFAULT 0
#define MENU_I_DEFAULT 0


// 세탁기 상태 관련 변수
bit power = OFF; 
volatile char state = OFF; // 세탁기 현재 상태
char state_tmp; // 일시 정지 시 상태 임시 저장
char time = 0; // 남은 시간, 정확한 1초 아님


// 모드 및 메뉴 관련 변수 
char mode_i = MODE_I_DEFAULT;
char menu_i = MENU_I_DEFAULT;
char mode[3][4] = {"STD", "SPD", "BED"}; // 표준 / 스피드 / 이불
char cnt_def[3] = {3, 2, 5}; // 세탁 / 헹굼 / 탈수 기본 횟수
char cnt[3] = {3, 3, 3};     // 세탁 / 헹굼 / 탈수 실제 횟수
char key = NO_INPUT;
char data[20]; // 문자열 버퍼


// 함수 선언
void pwm_speed(char dir);
void print_process(char process[]);
void pause();  
void final();  // 세탁기 변수 초기화 - init이 좋으나 인터럽트 내부에서 실행으로 인해 버그 발생
char input(); 


// 전원 인터럽트
interrupt [EXT_INT7] void ext_int7_isr(void)
{
	power = power ? OFF : ON;
    
    // 디바운싱 
    key = EXT_INT7;    
    while (INTERRUPT_BUTTON_PIN == key && key != NO_INPUT);
    
    if (power == OFF) {	
        final(); 
    }   
    else {
        state = MODE_SELECT;
    }
    
    key = NO_INPUT;                   
}
/*
void test() {
    power = ON;

    time += cnt[0] * 4;
    time += cnt[1] * 3;
    time += cnt[2] * 5;  
                        
    key = NO_INPUT;
    state = WASHING;
}
*/

void main(void)
{
    char i = 0;

	// 포트 입출력, 풀업 설정
	DDRB = 0x03; DDRF = 0x00; DDRG = 0x01;
	PORTB = 0xff; PORTF = 0xff; PORTG = 0x00;
	                       
	// 스위치 인터럽트(PORTE 7)
	EICRB = 0x80;
	EIMSK = 0x80;
	EIFR = 0x80;
    
	twi_master_init(100);
    #asm("sei");
	lcd_twi_init(0x27, 16);  
    
    //////////// 디버깅 
    //test();   
    //////////////////

    
	while(1) { 
        key = input();
                       
        if (power == ON) { 
            // 메뉴 선택
            if (state == MODE_SELECT) {
                if (key == NO_INPUT) {
                    lcd_gotoxy(0, 0);
                    sprintf(data, "MODE:%s", mode[mode_i]);
                    lcd_puts(data);
                        
                    lcd_gotoxy(0, 1);
                    sprintf(data, "WSH%d:RIN%d:DRY%d", cnt[0], cnt[1], cnt[2]);
                    lcd_puts(data);                
                }       
                // 표준 -> 스피드 -> 이불 모드 이동
                else if (key == MODE_BUTTON) {
                    menu_i = MENU_I_DEFAULT; // 모드 변경 시 세탁부터 횟수 증가
                    
                    mode_i++; 
                    if (mode_i == 3) mode_i = MODE_I_DEFAULT; 
                    memset(cnt, cnt_def[mode_i], 3); // 표준 : 3 / 스피드 : 2 / 이불 : 5
                }  
                else if (key == INCREASE_BUTTON) {
                    cnt[menu_i]++;  
                    if (cnt[menu_i] >= 6) cnt[menu_i] = 0; // 최대 횟수 5번     
                    
                }
                else if (key == START_PAUSE_BUTTON) {
                    time += cnt[0] * 4; // 세탁 1회 당 4초
                    time += cnt[1] * 3; // 헹굼 1회 당 3초
                    time += cnt[2] * 5; // 탈수 1회 당 5초 
                    
                    if (time != 0) { 
                        delay_ms(500); 
                        
                        key = NO_INPUT;  
                        state = WASHING;
                    }         
                }      
                // 세탁 -> 헹굼 -> 탈수 메뉴 이동
                else if (key == NEXT_BUTTON) { 
                    menu_i++;
                    if (menu_i % 3 == 0) menu_i = MENU_I_DEFAULT;
                }
            }         
                                       
            // 세탁 시작
            // cnt : 세탁 / 헹굼 / 탈수 횟수 배열
            else if (state == WASHING) { 
                for (i = 0; i < cnt[0] * 4; i++) {  
                    time--; 
                    
                    pwm_speed(LEFT);
                    
                    sprintf(data, "%s WASHING...", mode[mode_i]);
                    print_process(data);
   
                    pause(); 
                    if (power == OFF) break;
                }  
                
                state = RINSING;
            }     
            
            else if (state == RINSING) {
                for (i = 0; i < cnt[1] * 3; i++) {       
                    time--;
                    
                    pwm_speed(RIGHT);
                                   
                    sprintf(data, "%s RINSING...", mode[mode_i]);
                    print_process(data);
                    
                    pause();  
                    if (power == OFF) break; 
                } 
                 
                state = DRYING;
            }           
            
            else if (state == DRYING) { 
                for (i = 0; i < cnt[2] * 5; i++) {
                    time--;
                    
                    pwm_speed(LEFT);
                
                    sprintf(data, "%s DRYING...", mode[mode_i]);
                    print_process(data);
                    
                    pause();
                    if (power == OFF) break;
                }         
                
                state = END;
            } 
            
            else if (state == END) { 
                MOTOR_PIN = STOP;
                
                lcd_clear();
                lcd_gotoxy(0, 0);
                sprintf(data, "   FINISHED!!");
                lcd_puts(data);
                
                for (i = 0; i < 3; i++) {
                    BUZZER_PIN = BUZZER_ON;
                    delay_ms(500);
                    BUZZER_PIN = BUZZER_OFF;
                    delay_ms(500);
                }
                
                delay_ms(2000);
                final();   
            }          
            
        }           
        // 전원이 꺼져 있을 시
        else lcd_clear(); 

    }
}

// 모드, 현재 상태(세탁/헹굼/탈수), 남은 시간을 출력
void print_process(char process[]) {    
    lcd_gotoxy(0, 0);
    lcd_puts(process);
                        
    lcd_gotoxy(0, 1);
    sprintf(data, "Remain Time:%3d", time);
    lcd_puts(data);
}

void pause() {    
    if (key == START_PAUSE_BUTTON) {  
        // 세탁기 상태 전환(시작 -> 일시 정지)
        state_tmp = state; 
        state = PAUSE;
        
        lcd_clear();  
                                            
        sprintf(data, "%s PAUSE...", mode[mode_i]);            
        print_process(data);
        
        delay_ms(500);    
    }   
                                         
    // 키 입력 폴링      
    while (state == PAUSE) {
        key = input();
        MOTOR_PIN = STOP;   
        
        if (key == START_PAUSE_BUTTON) {
            state = state_tmp; // 세탁기 상태 전환(일시 정지 -> 시작)
            key = NO_INPUT;     
        }  
        
        if (power == OFF) return;
    }       
}


void pwm_speed(char dir) {
    char i = 0;   
                        
    while (i < 10){ // sec 단위 반복 (정확한 1초 아님)
        // 키 입력
        if (key != START_PAUSE_BUTTON) key = input();
           
        MOTOR_PIN=dir; 
        delay_ms(90);
  	MOTOR_PIN=STOP;
    	delay_ms(10);
	    
        i++;
        
        if (power == OFF) break;  
    }
}

void final() {
    lcd_clear();  
                     
    // 기본 상태 : 표준
    mode_i = MODE_I_DEFAULT;
    menu_i = MENU_I_DEFAULT;
    memset(cnt, cnt_def[0], 3);    
    MOTOR_PIN = STOP;  
    time = 0;
    
    state = OFF;
    power = OFF; 
}

char input() {
    key = BUTTON_PIN;     

    // 디바운싱
    while (key == BUTTON_PIN && key != NO_INPUT); 

    return key;
}
