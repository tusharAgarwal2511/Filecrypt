# #!/usr/bin/env bash

# # Directory where files should be created
# output_dir="test"

# # Ensure the folder exists
# mkdir -p "$output_dir"

# # Create 100 text files in the test directory
# for i in $(seq 1 10000); do
#   filename="${output_dir}/file${i}.txt"
#   tr -dc 'A-Za-z0-9' </dev/urandom | head -c 100000 > "$filename"
# done


#!/usr/bin/env bash

# Directory where files should be created
output_dir="test"

# Number of files to generate
num_files=10000

# Number of characters per file
num_chars=100000

# Ensure the folder exists
mkdir -p "$output_dir"

# Create text files in the test directory
for i in $(seq 1 $num_files); do
  filename="${output_dir}/file${i}.txt"
  tr -dc 'A-Za-z0-9' </dev/urandom | head -c $num_chars > "$filename"
done
