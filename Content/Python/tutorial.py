!pip install selenium webdriver-manager

from webdriver_manager.chrome import ChromeDriverManager
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
import os
import urllib.request
import time

# 크롬 옵션 설정
chrome_options = Options()
chrome_options.add_argument('--headless')  # 필요한 경우 헤드리스 모드 사용
service = Service(ChromeDriverManager().install())
driver = webdriver.Chrome(service=service, options=chrome_options)

URL = 'http://www.google.co.kr/imghp'

# 사용자 입력 받기
search_query = input("검색어를 입력하세요 (예: <카테고리> <이미지이름>): ")
image_count = int(input("다운로드할 이미지 개수를 입력하세요: "))

# 검색어를 공백으로 분리
keywords = search_query.split()
if len(keywords) >= 2:
    main_category = keywords[0]  # 첫 번째 단어는 상위 폴더명
    sub_category = keywords[1]   # 두 번째 단어는 하위 폴더명
else:
    main_category = "기타"
    sub_category = search_query

print("[1/6] 크롬 드라이버 초기화 완료")
driver.get(url=URL)
driver.implicitly_wait(time_to_wait=10)
print("[2/6] 구글 이미지 검색 페이지 접속 완료")

#검색창 찾기 (name 속성 사용)
elem = driver.find_element(By.NAME, "q")
elem.send_keys(search_query)
elem.send_keys(Keys.RETURN)
print(f"[3/6] 검색어 '{search_query}' 입력 완료")

import time
elem =driver.find_element(By.TAG_NAME, "body")
for i in range(60):
    elem.send_keys(Keys.PAGE_DOWN)
    time.sleep(0.1)
print("[4/6] 첫 번째 스크롤 완료")

try:
    # '결과 더보기' 버튼 찾기 (여러 선택자 시도)
    try:
        more_button = driver.find_element(By.CSS_SELECTOR, "input[type='button'][value='결과 더보기']")
    except:
        try:
            more_button = driver.find_element(By.CSS_SELECTOR, "input.mye4qd")
        except:
            try:
                more_button = driver.find_element(By.XPATH, "//input[@value='결과 더보기']")
            except:
                print("결과 더보기 버튼을 찾을 수 없습니다. 스크롤을 계속 진행합니다.")
                more_button = None

    if more_button and more_button.is_displayed():
        more_button.click()
        print("결과 더보기 버튼 클릭 완료")

        for i in range(60):
            elem.send_keys(Keys.PAGE_DOWN)
            time.sleep(0.1)
        print("[5/6] 두 번째 스크롤 완료")
    else:
        print("결과 더보기 버튼이 없거나 이미 모든 결과가 로드되었습니다.")

except Exception as e:
    print(f"스크롤 진행 중 오류 발생: {str(e)}")
    print("기본 검색 결과만 수집합니다.")

# 이미지 URL 수집 및 다운로드
links = []
print("\n이미지 URL 수집 중...")

try:
    images = driver.find_elements(By.TAG_NAME, "img")
    print(f"발견된 이미지 수: {len(images)}개")
    
    # 처음 4개의 이미지는 건너뛰기
    for image in images[5:]:  # 5번째 이미지부터 시작
        try:
            src = image.get_attribute('src')
            if src and src.startswith('http'):
                links.append(src)
                if len(links) >= image_count:  # 원하는 개수만큼만 수집
                    break
        except Exception as e:
            continue
            
    print(f'수집된 유효한 이미지 URL: {len(links)}개')
    print("[6/6] 이미지 다운로드를 시작합니다...")
    
    # 상위/하위 폴더 생성
    download_path = os.path.join(os.getcwd(), "crawling_img", main_category, sub_category)
    os.makedirs(download_path, exist_ok=True)
    
    # 이미지 다운로드
    for i, url in enumerate(links):
        if i >= image_count:  # 원하는 개수만큼만 다운로드
            break
        try:
            urllib.request.urlretrieve(url, os.path.join(download_path, f"{sub_category}_{i+1}.jpg"))
            if (i + 1) % 10 == 0:
                print(f"진행률: {i+1}/{image_count} 완료")
        except Exception as e:
            print(f"{i+1}번째 이미지 다운로드 실패: {str(e)}")
            continue

except Exception as e:
    print(f"이미지 수집 중 오류 발생: {str(e)}")

print('모든 이미지 다운로드 완료!')
print(f'저장 위치: {download_path}')
driver.quit()