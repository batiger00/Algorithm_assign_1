import os
import random

# 출력 디렉토리 설정
output_dir = "sorting_datasets"
os.makedirs(output_dir, exist_ok=True)

# 설정값
sizes = [1000, 10000, 100000, 500000, 1000000]
data_types = ["ascending", "descending", "random", "reverse", "partial"]
trials_per_type = 10

# 부분 정렬 생성 함수 (예: 70% 정렬)
def generate_partially_sorted(arr, percent_sorted=0.7):
    n_sorted = int(len(arr) * percent_sorted)
    arr[:n_sorted] = sorted(arr[:n_sorted])
    return arr

for size in sizes:
    for data_type in data_types:
        for trial in range(trials_per_type):
            # 데이터 생성
            if data_type == "ascending":
                data = list(range(size))
            elif data_type == "descending":
                data = list(range(size, 0, -1))
            elif data_type == "random":
                data = random.sample(range(size * 2), size)
            elif data_type == "reverse":
                data = list(range(size))[::-1]  # 동일한 descending과 구분되는 명확성 위해 유지
            elif data_type == "partial":
                data = list(range(size))
                random.shuffle(data)
                data = generate_partially_sorted(data)
            else:
                continue

            # 파일 이름 및 저장
            file_name = f"{data_type}_{size}_{trial+1}.txt"
            file_path = os.path.join(output_dir, file_name)
            with open(file_path, 'w') as f:
                f.write(" ".join(map(str, data)))

print(f"✅ 총 {len(sizes) * len(data_types) * trials_per_type}개의 데이터셋 생성 완료.")
