import os
from PIL import Image

def is_color_in_range(color, lower_bound, upper_bound):
    """Check if a color is within the specified range."""
    return all(lower_bound[i] <= color[i] <= upper_bound[i] for i in range(3))

def interpolate_color(color, lower_bound, upper_bound, new_lower_bound, new_upper_bound):
    """Interpolate the original color to the new color range."""
    return tuple(
        int(new_lower_bound[i] + (color[i] - lower_bound[i]) * (new_upper_bound[i] - new_lower_bound[i]) / (upper_bound[i] - lower_bound[i]))
        for i in range(3)
    )

def alter_image_color(image_path, lower_bound, upper_bound, new_lower_bound, new_upper_bound):
    with Image.open(image_path) as img:
        img = img.convert("RGBA")
        data = img.getdata()

        # Alter the colors
        new_data = []
        for item in data:
         
            if is_color_in_range(item[:3], lower_bound, upper_bound):
               
                new_color = interpolate_color(item[:3], lower_bound, upper_bound, new_lower_bound, new_upper_bound)
                new_data.append((*new_color, item[3]))
            else:
                new_data.append(item)

        img.putdata(new_data)
        img.save(image_path) 

def process_directory(directory, lower_bound, upper_bound, new_lower_bound, new_upper_bound):
    for root, _, files in os.walk(directory):
        for file in files:
            if file.lower().endswith(".png"):
                image_path = os.path.join(root, file)
                alter_image_color(image_path, lower_bound, upper_bound, new_lower_bound, new_upper_bound)
                print(f"Processed {image_path}")


# lower_bound = (30, 25, 24)  # Lower bound of the original color range 
# upper_bound = (130, 110, 141)  # Upper bound of the original color range 
# new_lower_bound = (25, 25, 25)#(62, 62, 62)  # Lower bound of the new color range 
# new_upper_bound =  (62, 62, 62) # Upper bound of the new color range
lower_bound = (30, 25, 24)#(25, 25, 25)
upper_bound = (130, 110, 141)# (62, 62, 62)

new_lower_bound = (20, 20, 20)
new_upper_bound = (65, 65, 65)
directory = "."


process_directory(directory, lower_bound, upper_bound, new_lower_bound, new_upper_bound)
