import torch
from torch import nn
import torch.optim as optim
import torchvision.transforms as transforms
import torchvision.models as models
import torchvision.datasets as datasets
from torch.utils.data import DataLoader
import pandas as pd
import cv2
import numpy as np

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
output_number=12

model = models.mobilenet_v3_large(pretrained=True)
model.classifier[3] = nn.Linear(model.classifier[3].in_features, output_number)  # For ImageNet, 1000 classes
model.to(device)
model.eval()  # Set the model to evaluation mode

# 2. Define preprocessing steps for frames
transform = transforms.Compose([
    transforms.ToPILImage(),
    transforms.Resize((224, 224)),  # Resize to match MobileNet input size
    transforms.ToTensor(),
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
])

train_dataset = datasets.ImageFolder(root='./Dataset', transform=transform)
train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)

# 2. Load the class label mapping (assuming class.csv contains class names in the same order)
class_mapping = pd.read_csv('class.csv')

# Print dataset size and class mapping
print(f"Number of samples: {len(train_dataset)}")
print(f"Class to index mapping: {train_dataset.class_to_idx}")


# Load pre-trained MobileNetV3 model
model = models.mobilenet_v3_large(pretrained=True)

# Modify the final layer to match the number of classes in your dataset
num_classes = len(train_dataset.classes)
model.classifier[3] = nn.Linear(model.classifier[3].in_features, num_classes)
model = model.to(device)


# Loss function and optimizer
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)

num_epochs = 10

for epoch in range(num_epochs):
    model.train()
    running_loss = 0.0

    for i, (inputs, labels) in enumerate(train_loader):
        inputs, labels = inputs.to(device), labels.to(device)

        # Zero the parameter gradients
        optimizer.zero_grad()

        # Forward pass
        outputs = model(inputs)
        loss = criterion(outputs, labels)

        # Backward pass and optimization
        loss.backward()
        optimizer.step()

        # Print statistics
        running_loss += loss.item()
        if i % 100 == 99:  # Print every 100 mini-batches
            print(f'Epoch [{epoch + 1}/{num_epochs}], Step [{i + 1}/{len(train_loader)}], Loss: {running_loss / 100:.4f}')
            running_loss = 0.0

print('Finished Training')

# Save the model
torch.save(model.state_dict(), 'model.pth')

# Testing the model
model.eval()
test_image = 'path_to_a_test_image.jpg'

# Pre-process the test image
from PIL import Image

image = Image.open(test_image)
image = transform(image).unsqueeze(0).to(device)

# Predict the class
with torch.no_grad():
    outputs = model(image)
    _, predicted = torch.max(outputs, 1)

print(f"Predicted Class: {train_dataset.classes[predicted.item()]}")
