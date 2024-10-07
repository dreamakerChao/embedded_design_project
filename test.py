import torch
import cv2
from torchvision import models, transforms
from PIL import Image

# 1. 加載 ResNet18 模型
model = models.resnet18()

# 2. 調整全連接層以匹配 10 個類別
num_classes = 10  # 你訓練的類別數
model.fc = torch.nn.Linear(model.fc.in_features, num_classes)

# 3. 加載保存的權重
model.load_state_dict(torch.load('model.pth'))
model.eval()  # 設置為評估模式
classes = ("biscoff","butter_cheese","cadina","crispiroll","hokkaido", "nothing","onion","scientist_noodles","snapu_cookies","soda")

# 2. 定義影像預處理步驟
preprocess = transforms.Compose([
    transforms.Resize((299, 299)),  # 調整影像大小
    transforms.ToTensor(),  # 轉換為Tensor
    transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225]),  # 正規化
])



# 4. 打開相機
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("無法打開相機")
    exit()

# 5. 從相機捕捉影像，並進行推理
while True:
    ret, frame = cap.read()  # 捕捉一幀影像

    if not ret:
        break  # 如果無法捕捉到影像，退出循環

    # 6. 將 OpenCV 影像轉換為 PIL 影像格式，然後進行預處理
    img = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
    input_tensor = preprocess(img).unsqueeze(0)  # 增加 batch 維度

    # 7. 將影像傳入模型進行推理
    with torch.no_grad():
        output = model(input_tensor)
        _, predicted = output.max(1)

    # 8. 從模型的預測結果中獲取類別標籤
    label = classes[predicted.item()]

    # 9. 在影像上顯示預測結果
    cv2.putText(frame, f'Predicted: {label}', (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    # 10. 顯示影像
    cv2.imshow('Camera', frame)

    # 按 'q' 鍵退出
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 11. 釋放相機資源並關閉所有窗口
cap.release()
cv2.destroyAllWindows()
