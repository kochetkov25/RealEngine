import os
import sys

FRAG_SUFFIX = "FragmentShader.frag"
VERT_SUFFIX = "VertexShader.vert"

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SHADER_DIR = os.path.join(SCRIPT_DIR, "shaders")
OUTPUT_FILE = os.path.join(SCRIPT_DIR, "out", "ShaderName.hpp")

ENUM_BEGIN = "// BEGIN_ENUM"
ENUM_END = "// END_ENUM"
MAP_BEGIN = "// BEGIN_MAP"
MAP_END = "// END_MAP"


def find_shader_pairs(shader_dir):
    files = os.listdir(shader_dir)
    frag_map = {}
    vert_map = {}

    for file in files:
        if file.endswith(FRAG_SUFFIX):
            name = file[:-len(FRAG_SUFFIX)]
            frag_map[name] = file
        elif file.endswith(VERT_SUFFIX):
            name = file[:-len(VERT_SUFFIX)]
            vert_map[name] = file
        elif file.endswith(".frag") or file.endswith(".vert"):
            raise RuntimeError(f"Invalid shader filename format: {file}")

    all_names = frag_map.keys() | vert_map.keys()
    shader_pairs = {}

    for name in all_names:
        if name not in frag_map or name not in vert_map:
            raise RuntimeError(f"Missing shader pair for: {name}")
        shader_pairs[name] = (frag_map[name], vert_map[name])

    return shader_pairs


def generate_enum(shader_pairs):
    lines = [f"enum class ShaderName {{"]
    for name in sorted(shader_pairs.keys()):
        lines.append(f"    {name.upper()},")
    lines.append("};")
    return "\n".join(lines)


def generate_map(shader_pairs):
    lines = [
        "inline const std::unordered_map<ShaderName, std::pair<std::string, std::string>> ShaderPaths = {"
    ]
    for name, (frag, vert) in sorted(shader_pairs.items()):
        lines.append(f'    {{ ShaderName::{name.upper()}, std::make_pair("{frag}", "{vert}") }},')
    lines.append("};")
    return "\n".join(lines)


def replace_between_markers(contents, begin_marker, end_marker, new_block):
    start = contents.find(begin_marker)
    end = contents.find(end_marker)
    if start == -1 or end == -1 or start >= end:
        raise RuntimeError(f"Markers '{begin_marker}' or '{end_marker}' not found or invalid")
    before = contents[:start + len(begin_marker)]
    after = contents[end:]
    return before + "\n" + new_block + "\n" + after


def main():
    try:
        if not os.path.isdir(SHADER_DIR):
            raise RuntimeError(f"Shader directory not found: {SHADER_DIR}")
        if not os.path.isfile(OUTPUT_FILE):
            raise RuntimeError(f"Header file not found: {OUTPUT_FILE}")

        shader_pairs = find_shader_pairs(SHADER_DIR)
        new_enum = generate_enum(shader_pairs)
        new_map = generate_map(shader_pairs)

        with open(OUTPUT_FILE, "r", encoding="utf-8") as f:
            contents = f.read()

        contents = replace_between_markers(contents, ENUM_BEGIN, ENUM_END, new_enum)
        contents = replace_between_markers(contents, MAP_BEGIN, MAP_END, new_map)

        with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
            f.write(contents)

        print(f"ShaderName.hpp successfully updated.")
    except Exception as e:
        print(f"ERROR: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
