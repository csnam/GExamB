## GExamB 
* 시작좌표(x1,y1), 종료좌표(x2,y2)를 입력 받을 수 있는 UI를 생성
* Draw 버튼 클릭시 (x1, y1) 좌표를 중심으로하는 랜덤한 크기의 원을 생성
* Action 버튼 클릭시 (x1, y1) 좌표에서 (x2, y2) 좌표로 이미지 수 설정에 따라 일정 픽셀 간격으로 원을 이동
* 이동할때마다 image 하위 폴더에 BMP파일 저장함  UI가 프리징되지 않도록 스레드로 처리  
* Load 버튼 클릭시  저장된 이미지를 선택하면 화면에 출력하고 원의 중심 좌표에 X 표시하고 값을 표시함


