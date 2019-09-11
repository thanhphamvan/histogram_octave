img = imread("duck2.jpeg")
result = histogram(img(:, :, 1))

size(result)
print(result)
