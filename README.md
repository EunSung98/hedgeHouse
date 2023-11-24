# hedgeHouse 고슴도치 스마트 케이지

## 소개
- 고슴도치가 맞는 환경을 구성하고 보호자를 위한 스마트 케이지 (IoT)
## 팀원 수
- 임베디드 : 3명
- 안드로이드 : 1명

## 핵심 기능
1. 스마트 케이지
- 온습도&밝기 측정
- 냉풍기&온풍기로 온도 조절하고 가습기로 습도 조절
- 펫 CCTV,스마트 커튼, 자동 급식기
2. 안드로이드 앱
- 케이지의 온습도와 밝기를 그래프로 확인
- 냉풍기와 온풍기, 가습기, 커튼 on/off 기능-펫 CCTV로 케이지 안 실시간으로 확인

## 역할 
요약 : 팀장, 임베디드 파트 (센서 및 부품 조작), 안드로이드와 firebase 연동

- 앱과 하드웨어 파트마다 프로젝트 관리 (기여도 : 80%)
- C++로 온습도 센서 값 측정 (기여도 : 100%)
- 냉풍기와 온풍기, 커튼을 조작 (기여도 : 80%)
- 센서값과 부품의 on/off 값을 Serial 통신으로 Arduino에서 esp32로 전달 (기여도 : 90%)
- esp32와 firebase의 데이터 송수신 (기여도 : 70%)
- Android Studio에 firebase Realtime database를 연동 (기여도 : 100%)
- firebase 데이터를 그래프로 시각화 (기여도 : 50%)

<hr>
## 요구 사항 정의서
![image](https://github.com/EunSung98/hedgeHouse/assets/77737044/e4a15d96-a619-40e7-a690-b3c4c0bd044a)

## 서비스 흐름도
![image](https://github.com/EunSung98/hedgeHouse/assets/77737044/a4d5ef35-1b58-4fe6-9f89-65808c8ae12d)

## 결과 페이지
![image](https://github.com/EunSung98/hedgeHouse/assets/77737044/63f848c3-74ed-4066-844d-ae2e8f6fdd09)

![image](https://github.com/EunSung98/hedgeHouse/assets/77737044/1ad66410-39f5-4ccd-b816-4becab933d34)

![image](https://github.com/EunSung98/hedgeHouse/assets/77737044/84c22589-94f9-4b8c-959c-0096c51926b6)

## 역량
- 프로젝트 계획이 중요성을 배움
- 포기하지 않았기에 좋은 경험과 결과가 됐다고 생각
- 팀장의 책임감으로 힘들었는데 나의 상황을 진솔하게 전해 팀원들이 더욱 적극적으로 임해 줘서 고마움을 느낌  
- 서로 힘든 일이 있으면 얘기해야 한다고 배움
- 인원 부족과 시간 상 우선순위를 필요한 기능이라도 구현

### hedgeCage.ino
- 아두이노 메가보드의 코드입니다.
- 주된 기능을 담당하는 메인 보드입니다.

### serial2.ino
- esp32의 코드입니다.
- firebase와 연동합니다.

<hr>
<참고>
아래 영상을 보시면 어떻게 작동되는지 알 수 있습니다.<br>
<img src="https://user-images.githubusercontent.com/77737044/210165701-d9f0729c-68a5-4f03-b9bc-3d74f9292110.png" width="400" height="200">
https://youtu.be/HR_xoc30NMU
