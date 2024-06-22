import random

def generate_test_data(num_computers, num_tasks, filename):
    try:
        with open(filename, 'w') as f:
            # 写入计算机数量
            f.write(f"{num_computers}\n")
            
            # 生成带宽数据
            bandwidths = [random.randint(1, 100) for _ in range(num_computers - 1)]
            f.write(" ".join(map(str, bandwidths)) + "\n")
            
            # 生成任务数据
            for _ in range(num_tasks):
                sender = random.randint(1, num_computers)
                receiver = random.randint(1, num_computers)
                while receiver == sender:  # 确保发送者和接收者不相同
                    receiver = random.randint(1, num_computers)
                workload = random.randint(1, 100)
                f.write(f"{sender} {receiver} {workload}\n")
        print(f"Test data generated successfully: {filename}")
    except Exception as e:
        print(f"An error occurred while generating test data: {e}")

# 逐步增加数据规模，确保生成成功
try:
    generate_test_data(64, 32, 'test_data_64.txt')  # 64规模，32个任务
    generate_test_data(128, 64, 'test_data_128.txt')  # 128规模，64个任务
    #generate_test_data(256, 128, 'test_data_256.txt')  # 256规模，128个任务
    #generate_test_data(10, 5, 'test_data_small.txt')    # 小规模
    #generate_test_data(100, 50, 'test_data_medium.txt') # 中等规模
    #generate_test_data(10000, 5000, 'test_data_large.txt') # 大规模
    #generate_test_data(100000, 50000, 'test_data_very_large.txt')  # 更大规模
    #generate_test_data(10000, 5000, 'test_data_10k_5k.txt')    # 10,000个光纤，5,000个任务
    #generate_test_data(50000, 25000, 'test_data_50k_25k.txt')  # 50,000个光纤，25,000个任务
    #generate_test_data(100000, 50000, 'test_data_100k_50k.txt')  # 100,000个光纤，50,000个任务
except Exception as e:
    print(f"Error during test data generation: {e}")
