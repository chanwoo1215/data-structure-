피보나치 수열 - 재귀적 방법
visual studio 2022 



n = 25 
f(25) = 75025

특이사항 : 함수가 자기 자신을 호출하는 구조로 인해 발생하는 리소스 변화 측정


최대 프로세스 메모리 : 796kb

cpu 점유율 특징 : 하단 그래프의 잦은 요동 [함수 호출/복귀 오버헤드 발생]

진단 세션 시간 17.944초 

cpu 사용량 그래프 : 일정한 높낮이를 가진 파동 관찰

이는 f(n) = f(n-1) + f(n-2)를 처리하기 위해 cpu가 끊임없이 함수를 호출하고 스택을 관리하며 발생하는 연산 부하를 나타냄

n이 커질수록 이 파동은 기하급수적으로 높아져 cput 100% 점유 상태에 도달

재귀적 방법의 문제점 
 시간 비효율성 : n이 커질수록 시간이 오래 걸림

 리소스 낭비 : 그래프에서 확인된 cpu의 잦은 변동은 불필요한 함수 호출 오버헤드를 증명

 메모리 오버헤드 : 미세하지만 스택 메모리 사용량이 증가하는 것을 데이터로 확인.



 <img width="1586" height="710" alt="스크린샷 2026-04-13 165243" src="https://github.com/user-attachments/assets/5cccb270-cd44-4bf2-85ad-8338405ed4e1" />
 <img width="1029" height="399" alt="스크린샷 2026-04-13 165318" src="https://github.com/user-attachments/assets/91408754-51c7-4124-bbb8-30fde467e01e" />
<img width="1304" height="411" alt="스크린샷 2026-04-13 165259" src="https://github.com/user-attachments/assets/1a0463d6-a011-45b1-b10d-1c7b6a53d136" />



