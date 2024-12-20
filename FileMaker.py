import os
import random
import string

def generate_random_content():
    paragraphs = random.randint(2, 5)
    content = []
    for _ in range(paragraphs):
        sentences = random.randint(3, 7)
        paragraph = []
        for _ in range(sentences):
            sentence_length = random.randint(5, 15)
            sentence = " ".join(
                "".join(random.choices(string.ascii_letters + " ", k=sentence_length))
                .strip()
                .capitalize()
                + "."
                for _ in range(random.randint(5, 15))
            )
            paragraph.append(sentence)
        content.append(" ".join(paragraph))
    return "\n\n".join(content)

def create_test_files(directory="test", num_files=100):
    os.makedirs(directory, exist_ok=True)
    for i in range(num_files):
        filename = f"file_{i + 1}.txt"
        filepath = os.path.join(directory, filename)
        with open(filepath, "w") as f:
            f.write(generate_random_content())
    print(f"{num_files} test files created in '{directory}' directory.")

if __name__ == "__main__":
    create_test_files()
