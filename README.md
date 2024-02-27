# 세탁기 제작 프로젝트

## 프로젝트 개요
- 강의명 : IoT 오픈 플랫폼 활용 
- 강의 기간 : 2024/01/04(목) ~ 2024/01/23(화)
- 프로젝트 이름 : 세탁기 제작 프로젝트
- 프로젝트 기간 : 2024/01/15(월) ~ 2024/01/22(월), 7일 간 제작

<br>

### 목표
- ATmega128a를 사용해 미니 세탁기 제작
- 임베디드 시스템을 구현해봄으로써 하드웨어 작동 방식에 적응
- 노션 및 draw.io 사용으로 협업 프로세스 적용
- 모듈 별로 한 명씩 담당해 기능 구현
- 상태 전이도 및 회로도 작성으로 동작 방식 학습과 회로 설계 능력 강화

  
  <br>


### 팀원
- 이용주
- 이인혁
- 정우택
- 한민규




<br><br>

## 프로젝트 환경

### 사용 부품

| ![ATmega128a](https://github.com/almondgood/Washing-Machine/assets/88735581/14b12adc-6239-48c8-8df0-69d1b5d73097)| ![I2C LCD](https://github.com/almondgood/Washing-Machine/assets/88735581/cc197e4a-92eb-4ce0-9ea7-562f6cc22dfd) | ![DC 모터](https://github.com/almondgood/Washing-Machine/assets/88735581/37966045-1458-4bc6-b353-00251b801471) | ![택트 스위치](https://github.com/almondgood/Washing-Machine/assets/88735581/8b9902a1-d49a-415b-87b2-f15497f5a6a6) | ![부저](https://github.com/almondgood/Washing-Machine/assets/88735581/4f5a7387-c93c-426c-9c8c-2e50c8f16697)|
|:------------------------:|:---:|:---:|:---:|:---:|
| ATmega128a | I2C LCD | DC 모터 | 택트 스위치 | 부저 |

<br>

### 하드웨어 구성도

![설계도](https://github.com/almondgood/Washing-Machine/assets/88735581/af003981-ffab-4320-aed5-f4d5cfc40453)

<br>
<br>

### 기술 스택

- 사용 언어
> ![C](https://img.shields.io/badge/C-ANSI_C-white?logo=c&logoColor=A8B9CC)

- 사용 IDE
> ![CodevisionAVR](https://img.shields.io/badge/CodevisionAVR-v4.01-white.svg?logo=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAAIGNIUk0AAHomAACAhAAA+gAAAIDoAAB1MAAA6mAAADqYAAAXcJy6UTwAAAKOUExURf////z7+/TMzPCWlu6BgeySkuS/v+ympuqenuzc3PC3t+2bm+PCwvjs7PJ5efocHPoCAvcAAPcBAa5GRucdHfQEBMyPj/7///HNzfcgIOYICLN+fvn09PBpaf0AANcICK1AQK9cXLRAQK6BgeUzM/sAAMhvb/X6+u5ra7g0NNjY2OypqfsNDeMFBat9fefs7Pb6+urt7enr6+FTU/4AAMpWVuW/v/kXF9wGBrmamudeXv8AAMcpKd/f3/z//954eMk0NNtRUfoAALFISOFGRtcqKuTa2v3///r29vjx8eCenvkDA+IICO8FBdAREcOystJwcPgAAPwDA+tBQfFra/FKSsqCgvr//+XAwPMPD/MAAK9gYPL19eHV1cVJSeIGBu4AAOsAANQCArmHh+vd3dkmJugAALwhIc/IyObl5cKnp7aEhLOEhLympujo6Pj398ewsLiXl8a5ufr9/f/9/fjY2PDY2Pnp6fTS0vj29v79/ffZ2e7Z2fXU1PjLy/bS0vXT0/MnJ+AqKt/S0uOmpuQfH9a0tPbf3+wwMKtQUOcuLtIYGN8eHuBcXOvm5vKIiMUHB9IHB9CJieXV1eQdHcptbeuhodcPD+0gIKhDQ9BERNATE87Bwffq6vM6OrMlJdYjI9U8POPl5d9cXNArK9pERLkvL9G+vvEiIt4QEOUNDaJRUejq6vOpqe4MDL4UFMwUFOUODte4uN+qquULC9wDA7Bzc+vZ2e4fH6M0NM8lJdhHR+3l5dFhYa4jI7OpqcWrq8UkJLh3d+fg4M04OLQbG8m+vvHi4sctLa91dc6goMMeHraBgczCwsW+vvn6+vv7+8m/v87Hx9DIyMK5ufT09Ma8vNfT08W6usjBwUaFrXEAAAABYktHRACIBR1IAAAAB3RJTUUH6AIXBg4bkaaX2QAAAIhJREFUGNNjZGBghIHvjJxAkoGXERXIIHO+cTOy8CMLCDDeAwscZmCwY/zKDWS9f8no8p0LJHfOGKxkIwMLD8RQB0bGR/JAKxhYoJZs8WXUZmScwcDAtJgbDF6CSQagqxhKGBn/ssCsObWdkaEB1V0s7nCH9JSCqbloLmcRIFIAZM+sdMZ/zIwATtgYrY9y0XYAAAAldEVYdGRhdGU6Y3JlYXRlADIwMjQtMDItMjNUMDY6MTQ6MjErMDA6MDBVr3uYAAAAJXRFWHRkYXRlOm1vZGlmeQAyMDI0LTAyLTIzVDA2OjE0OjIxKzAwOjAwJPLDJAAAACh0RVh0ZGF0ZTp0aW1lc3RhbXAAMjAyNC0wMi0yM1QwNjoxNDoyNyswMDowMBA318EAAAAASUVORK5CYII=)



<br>

## 프로젝트 과정
![image](https://github.com/almondgood/Washing-Machine/assets/88735581/51ea52fb-9538-490b-93a0-caafa7d119e3)



## 기능

### 스위치 구성
![image](https://github.com/almondgood/Washing-Machine/assets/88735581/bf6824b3-f47a-4eaa-8f55-1cdfb17f20f8)


<br>


### 작동 방식


| <video src="https://github.com/almondgood/Washing-Machine/assets/88735581/e2fabd24-1c51-4e8c-b7f2-d83c88fa4ef5"> |  <video src="https://github.com/almondgood/Washing-Machine/assets/88735581/091f5c58-1dd6-4dc6-b462-be3af83c4b1e">| <video src="https://github.com/almondgood/Washing-Machine/assets/88735581/e65a4923-240b-47dc-bd92-05b99178cc2c"> |
|:---:|:---:|:---:|
|1. 전원 ON|2. 전원 OFF|3. 동작 중 전원 ON/OFF |


<br><br>

|<video src="https://github.com/almondgood/Washing-Machine/assets/88735581/de50a6c4-1246-4538-9628-8bf4e3ab9902"> |<video src="https://github.com/almondgood/Washing-Machine/assets/88735581/32b810ab-fbed-4054-8242-24155c28f4f4"> |<video src="https://github.com/almondgood/Washing-Machine/assets/88735581/f8120a0c-b2f0-4a49-97d3-2ba5c9dda3de"> |
|:---:|:---:|:---:|
|4. 모드 선택|5. 메뉴 및 횟수 선택|6. 시작 |

<br><br>

|<video src="https://github.com/almondgood/Washing-Machine/assets/88735581/5cb63a4e-384b-4c4c-9c85-a00f6ef5a004"> |<video src="https://github.com/almondgood/Washing-Machine/assets/88735581/64e5d71f-da23-4b9f-8de5-eb2bc836efef"> |<video src="https://github.com/almondgood/Washing-Machine/assets/88735581/f6954a96-4b84-47ae-9210-3a002636b5bb"> | <video src="https://github.com/almondgood/Washing-Machine/assets/88735581/a7201640-b38a-4b01-b6a4-0d72bf8b8d42">|
|:---:|:---:|:---:|:---:|
|7. 세탁 |8. 헹굼|9. 탈수|10. 완료|


<br><br>


## 결론

- 세탁기의 주요 기능들은 대부분 구현(전원, 모드, 횟수, 세탁, 헹굼, 탈수, 알림 등)
- 바운싱은 소프트웨어적으로 80% 정도 해결
- 전원은 인터럽트로 구현했으나 바운싱으로 인해 일관성 있게 구현된 코드는 아님
- 시작/정지 버튼은 폴링으로 처리
- IDE의 한계로 애초 구현했던 모터 속도, 부저 음악 등을 적용하지 못함
- 타이머 미사용으로 1초가 정확하지 않음






