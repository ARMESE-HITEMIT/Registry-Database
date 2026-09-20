import os
import shutil
import subprocess
import platform
import socket
from pathlib import Path
import threading
import sys
import tempfile
import time
from datetime import datetime
import ctypes
from ctypes import wintypes
import shutil

socket.setdefaulttimeout(30)

def get_mode(path):
    attr = ctypes.windll.kernel32.GetFileAttributesW(path)
    if attr == 0xFFFFFFFF:
        return "?"
    return (
        ("d" if attr & 0x10 else "-") +
        ("a" if attr & 0x20 else "-") +
        ("r" if attr & 0x1 else "-") +
        ("h" if attr & 0x2 else "-") +
        ("s" if attr & 0x4 else "-") +
        ("l" if attr & 0x400 else "-")
    )

class ANSI:
    RESET = "\033[0m"
    BOLD = "\033[1m"
    DIM = "\033[2m"
    ITALIC = "\033[3m"
    UNDERLINE = "\033[4m"
    BLINK = "\033[5m"
    FAST_BLINK = "\033[6m"
    REVERSE = "\033[7m"
    HIDDEN = "\033[8m"
    STRIKETHROUGH = "\033[9m"
    BLACK = "\033[30m"
    RED = "\033[31m"
    GREEN = "\033[32m"
    YELLOW = "\033[33m"
    BLUE = "\033[34m"
    MAGENTA = "\033[35m"
    CYAN = "\033[36m"
    WHITE = "\033[37m"
    GRAY = "\033[90m"
    BRIGHT_BLACK = "\033[90m"
    BRIGHT_RED = "\033[91m"
    BRIGHT_GREEN = "\033[92m"
    BRIGHT_YELLOW = "\033[93m"
    BRIGHT_BLUE = "\033[94m"
    BRIGHT_MAGENTA = "\033[95m"
    BRIGHT_CYAN = "\033[96m"
    BRIGHT_WHITE = "\033[97m"

GLOBAL_ROOT = Path.cwd()
PROJECT_ROOT = GLOBAL_ROOT / "Program Execute Files"
INCLUDE_ROOT = GLOBAL_ROOT / "include"
BUILD_SCRIPT = {
    "build": {
        "msvc": [
            {
                "format_version": "1.0A",
                "type": "build",
                "metadata": {
                    "build": "cpp",
                    "inputs": [
                        {"type": "cpp", "input": r"./Program Files/registry_editor host/registry_editor host.cpp"}
                    ],
                    "flags": ["/O2", "/std:c++20"],
                    "output": r"./Program Execute Files/registry_editor host",
                    "build_target": "executable",
                    "build_type": "release"
                }
            },
            #{
            #    "format_version": "1.0A",
            #    "type": "build",
            #    "metadata": {
            #        "build": "cpp",
            #        "inputs": [
            #            {"type": "cpp", "input": r"./Program Files/shell_registry_editor/main.cpp"}
            #        ],
            #        "flags": ["/O2", "/std:c++20"],
            #        "output": r"./Program Execute Files/shell-dregedit",
            #        "build_target": "executable",
            #        "build_type": "release"
            #    }
            #},
            {
                "format_version": "1.0A",
                "type": "build",
                "metadata": {
                    "build": "cpp",
                    "inputs": [
                        {"type": "cpp", "input": r"./Program Files/registry_editor_service_local_dynamic_library/registry_editor_service_local.cpp"}
                    ],
                    "flags": ["/O2", "/std:c++20"],
                    "output": r"./include/registry_editor_service_local",
                    "build_target": "dynamic",
                    "build_type": "release"
                }
            },
            {
                "format_version": "1.0A",
                "type": "build",
                "metadata": {
                    "build": "cpp",
                    "inputs": [
                        {"type": "cpp", "input": r"./Program Files/registry_editor_service_local_static_library/registry_editor_service_local.cpp"}
                    ],
                    "flags": ["/O2", "/std:c++20"],
                    "output": r"./include/registry_editor_service_local",
                    "build_target": "static",
                    "build_type": "release"
                }
            },
            {
                "format_version": "1.0A",
                "type": "build",
                "metadata": {
                    "build": "cpp",
                    "inputs": [
                        {"type": "cpp", "input": r"./Program Files/registry_editor_dynamic_library/registry_editor.cpp"}
                    ],
                    "flags": ["/O2", "/std:c++20"],
                    "output": r"./include/registry_editor",
                    "build_target": "dynamic",
                    "build_type": "release"
                }
            },
            {
                "format_version": "1.0A",
                "type": "build",
                "metadata": {
                    "build": "cpp",
                    "inputs": [
                        {"type": "cpp", "input": r"./Program Files/registry_editor_static_library/registry_editor.cpp"}
                    ],
                    "flags": ["/O2", "/std:c++20"],
                    "output": r"./include/registry_editor",
                    "build_target": "static",
                    "build_type": "release"
                }
            }
        ],
        "other": [
            {
                "format_version": "1.0A",
                "type": "build",
                "metadata": {
                    "build": "cpp",
                    "inputs": [
                        {"type": "cpp", "input": r"./Program Files/registry_editor host/registry_editor host.cpp"}
                    ],
                    "flags": ["-O2", "-std=c++20"],
                    "output": r"./Program Execute Files/registry_editor host",
                    "build_target": "executable",
                    "build_type": "release"
                }
            },
            #{
            #    "format_version": "1.0A",
            #    "type": "build",
            #    "metadata": {
            #        "build": "cpp",
            #        "inputs": [
            #            {"type": "cpp", "input": r"./Program Files/shell_registry_editor/main.cpp"}
            #        ],
            #        "flags": ["-O2", "-std=c++20"],
            #        "output": r"./Program Execute Files/shell-dregedit",
            #        "build_target": "executable",
            #        "build_type": "release"
            #    }
            #},
            {
                "format_version": "1.0A",
                "type": "build",
                "metadata": {
                    "build": "cpp",
                    "inputs": [
                        {"type": "cpp", "input": r"./Program Files/registry_editor_service_local_dynamic_library/registry_editor_service_local.cpp"}
                    ],
                    "flags": ["-O2", "-std=c++20", "-fPIC"],
                    "output": r"./include/registry_editor_service_local",
                    "build_target": "dynamic",
                    "build_type": "release"
                }
            },
            {
                "format_version": "1.0A",
                "type": "build",
                "metadata": {
                    "build": "cpp",
                    "inputs": [
                        {"type": "cpp", "input": r"./Program Files/registry_editor_service_local_static_library/registry_editor_service_local.cpp"}
                    ],
                    "flags": ["-O2", "-std=c++20"],
                    "output": r"./include/registry_editor_service_local",
                    "build_target": "static",
                    "build_type": "release"
                }
            },
            {
                "format_version": "1.0A",
                "type": "build",
                "metadata": {
                    "build": "cpp",
                    "inputs": [
                        {"type": "cpp", "input": r"./Program Files/registry_editor_dynamic_library/registry_editor.cpp"}
                    ],
                    "flags": ["-O2", "-std=c++20", "-fPIC"],
                    "output": r"./include/registry_editor",
                    "build_target": "dynamic",
                    "build_type": "release"
                }
            },
            {
                "format_version": "1.0A",
                "type": "build",
                "metadata": {
                    "build": "cpp",
                    "inputs": [
                        {"type": "cpp", "input": r"./Program Files/registry_editor_static_library/registry_editor.cpp"}
                    ],
                    "flags": ["-O2", "-std=c++20"],
                    "output": r"./include/registry_editor",
                    "build_target": "static",
                    "build_type": "release"
                }
            }
        ]
    },
    "global_variable": [
        PROJECT_ROOT
    ]
}

def get_platform():
    system = platform.system()
    os_id = {"Windows": 1, "Linux": 2}.get(system, 0)
    machine = platform.machine().lower()
    arch_id = {"x86_64": 1, "amd64": 1, "x64": 1}.get(machine, 0)
    return [os_id, arch_id]

def _get_platform_name():
    system = platform.system()
    if system == "Windows": return "Windows"
    if system == "Linux": return "Unix"
    raise RuntimeError("[ERROR] Unsupported operating system. Only Windows and Linux are supported.")

def _get_platform_arch():
    machine = platform.machine().lower()
    if machine in ("x86_64", "amd64", "x64"): return "x86_64"
    raise RuntimeError("[ERROR] Unsupported CPU architecture. Only x64 is supported.")

def resolve_ext(path_str, item_type, platform_name):
    if not path_str: return path_str
    ext = ""
    t = str(item_type).lower()
    if t in ["c", "src_c"]: ext = ".c"
    elif t in ["cpp", "src_cpp", "cxx"]: ext = ".cpp"
    elif t == "python": ext = ".py"
    elif t in ["javascript", "js"]: ext = ".js"
    elif t == "bat": ext = ".bat"
    elif t == "ps1": ext = ".ps1"
    elif t == "ps2": ext = ".ps2"
    elif t == "executable": ext = ".exe" if platform_name == "Windows" else ""
    elif t == "dynamic": ext = ".dll" if platform_name == "Windows" else ".so"
    elif t == "static": ext = ".lib" if platform_name == "Windows" else ".a"
    elif t == "obj": ext = ".obj" if platform_name == "Windows" else ".o"
    elif t == "asm": ext = ".asm" if platform_name == "Windows" else ".s"

    if ext and not path_str.endswith(ext):
        return path_str + ext
    return path_str

class SUPPORTED_FORMAT_VERSIONS:
    class F1_0_A:
        class BUILD:
            _msvc_environment = None

            @staticmethod
            def _run_build_command(command, stage, environment=None):
                print()
                print(ANSI.BOLD + ANSI.CYAN + f"[BUILD] {stage}" + ANSI.RESET)
                print(f"  Command      : {' '.join(str(x) for x in command)}")
                print()

                try:
                    process = subprocess.Popen(
                        command,
                        stdin=None,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE,
                        cwd=None,
                        env=environment,
                        shell=False,
                        text=True,
                        errors="replace",
                        bufsize=1
                    )
                except Exception as e:
                    raise RuntimeError(
                        f"[BUILD ERROR] {stage}: error running compiler: {e}"
                    ) from e

                stdout_lines = []
                stderr_lines = []

                def read_stdout():
                    try:
                        for line in iter(process.stdout.readline, ""):
                            if not line:
                                break
                            stdout_lines.append(line)
                            print(line, end="", flush=True)
                    except Exception as e:
                        stderr_lines.append(
                            f"[BUILD OUTPUT ERROR] stdout: {e}\n"
                        )

                def read_stderr():
                    try:
                        for line in iter(process.stderr.readline, ""):
                            if not line:
                                break
                            stderr_lines.append(line)
                            print(line, end="", file=sys.stderr, flush=True)
                    except Exception as e:
                        stderr_lines.append(
                            f"[BUILD OUTPUT ERROR] stderr: {e}\n"
                        )

                stdout_thread = threading.Thread(
                    target=read_stdout,
                    daemon=False
                )
                stderr_thread = threading.Thread(
                    target=read_stderr,
                    daemon=False
                )

                stdout_thread.start()
                stderr_thread.start()

                return_code = process.wait()

                stdout_thread.join()
                stderr_thread.join()

                stdout = "".join(stdout_lines)
                stderr = "".join(stderr_lines)

                print()

                if return_code == 0:
                    print(
                        ANSI.GREEN +
                        f"[BUILD OK] {stage}" +
                        ANSI.RESET
                    )
                else:
                    print(
                        ANSI.RED +
                        f"[BUILD FAILED] {stage}" +
                        ANSI.RESET,
                        file=sys.stderr
                    )

                return subprocess.CompletedProcess(
                    command,
                    return_code,
                    stdout,
                    stderr
                )

            @staticmethod
            def _find_vswhere():
                candidates = []

                found = shutil.which("vswhere.exe") or shutil.which("vswhere")
                if found:
                    candidates.append(Path(found))

                drives = [
                    os.environ.get("SystemDrive", "C:"),
                    "C:",
                    "D:",
                    "E:",
                    "F:"
                ]

                seen_drives = []

                for d in drives:
                    clean_d = d.rstrip(":\\/") + ":\\"
                    if clean_d not in seen_drives:
                        seen_drives.append(clean_d)

                for env_var in [
                    "ProgramFiles(x86)",
                    "ProgramFiles",
                    "ProgramW6432"
                ]:
                    pf = os.environ.get(env_var)
                    if pf:
                        candidates.append(
                            Path(pf) /
                            "Microsoft Visual Studio" /
                            "Installer" /
                            "vswhere.exe"
                        )

                for d in seen_drives:
                    candidates.append(
                        Path(d) /
                        "Program Files (x86)" /
                        "Microsoft Visual Studio" /
                        "Installer" /
                        "vswhere.exe"
                    )

                    candidates.append(
                        Path(d) /
                        "Program Files" /
                        "Microsoft Visual Studio" /
                        "Installer" /
                        "vswhere.exe"
                    )

                seen = set()

                for candidate in candidates:
                    try:
                        key = str(candidate).lower()

                        if key in seen:
                            continue

                        seen.add(key)

                        if candidate.is_file():
                            return candidate
                    except OSError:
                        pass

                return None

            @staticmethod
            def _find_msvc_environment():
                if (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._msvc_environment
                    is not None
                ):
                    return (
                        SUPPORTED_FORMAT_VERSIONS
                        .F1_0_A
                        .BUILD
                        ._msvc_environment
                    )

                if platform.system().lower() != "windows":
                    return None

                existing_cl = shutil.which("cl.exe")

                if (
                    existing_cl
                    and os.environ.get("INCLUDE")
                    and os.environ.get("LIB")
                ):
                    environment = os.environ.copy()
                    environment["PATH"] = (
                        str(Path(existing_cl).parent) +
                        os.pathsep +
                        environment.get("PATH", "")
                    )

                    (
                        SUPPORTED_FORMAT_VERSIONS
                        .F1_0_A
                        .BUILD
                        ._msvc_environment
                    ) = environment

                    return environment

                vcvars_candidates = []

                vswhere = (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._find_vswhere()
                )

                if vswhere is not None:
                    vswhere_args_list = [
                        [
                            str(vswhere),
                            "-latest",
                            "-products",
                            "*",
                            "-requires",
                            "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
                            "-property",
                            "installationPath"
                        ],
                        [
                            str(vswhere),
                            "-all",
                            "-products",
                            "*",
                            "-requires",
                            "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
                            "-property",
                            "installationPath"
                        ],
                        [
                            str(vswhere),
                            "-latest",
                            "-prerelease",
                            "-products",
                            "*",
                            "-property",
                            "installationPath"
                        ],
                        [
                            str(vswhere),
                            "-all",
                            "-prerelease",
                            "-products",
                            "*",
                            "-property",
                            "installationPath"
                        ]
                    ]

                    for v_args in vswhere_args_list:
                        try:
                            result = subprocess.run(
                                v_args,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                text=True,
                                encoding="utf-8",
                                errors="replace",
                                shell=False
                            )

                            if result.returncode != 0:
                                continue

                            if not result.stdout:
                                continue

                            for line in result.stdout.splitlines():
                                install_path = line.strip()

                                if not install_path:
                                    continue

                                ip = Path(install_path)

                                if not ip.is_dir():
                                    continue

                                c1 = (
                                    ip /
                                    "VC" /
                                    "Auxiliary" /
                                    "Build" /
                                    "vcvars64.bat"
                                )

                                c2 = (
                                    ip /
                                    "VC" /
                                    "Auxiliary" /
                                    "Build" /
                                    "vcvarsall.bat"
                                )

                                if c1.is_file():
                                    vcvars_candidates.append((c1, ""))

                                if c2.is_file():
                                    vcvars_candidates.append((c2, "amd64"))

                        except OSError:
                            pass

                env_keys = [
                    "VSINSTALLDIR",
                    "VCINSTALLDIR",
                    "VS2026INSTALLDIR",
                    "VS2025INSTALLDIR",
                    "VS2022INSTALLDIR",
                    "VS2019INSTALLDIR",
                    "VS2017INSTALLDIR",
                    "VS180COMNTOOLS",
                    "VS170COMNTOOLS",
                    "VS160COMNTOOLS",
                    "VS150COMNTOOLS",
                    "VS140COMNTOOLS"
                ]

                for key in env_keys:
                    val = os.environ.get(key)

                    if not val:
                        continue

                    p = Path(val)

                    base = (
                        p.parent.parent
                        if "COMNTOOLS" in key
                        else p
                    )

                    c1 = (
                        base /
                        "VC" /
                        "Auxiliary" /
                        "Build" /
                        "vcvars64.bat"
                    )

                    c2 = (
                        base /
                        "VC" /
                        "Auxiliary" /
                        "Build" /
                        "vcvarsall.bat"
                    )

                    if c1.is_file():
                        vcvars_candidates.append((c1, ""))

                    if c2.is_file():
                        vcvars_candidates.append((c2, "amd64"))

                try:
                    import winreg

                    reg_keys = [
                        (
                            winreg.HKEY_LOCAL_MACHINE,
                            r"SOFTWARE\Microsoft\VisualStudio\SxS\VS"
                        ),
                        (
                            winreg.HKEY_LOCAL_MACHINE,
                            r"SOFTWARE\WOW6432Node\Microsoft\VisualStudio\SxS\VS"
                        ),
                        (
                            winreg.HKEY_CURRENT_USER,
                            r"SOFTWARE\Microsoft\VisualStudio\SxS\VS"
                        ),
                        (
                            winreg.HKEY_CURRENT_USER,
                            r"SOFTWARE\WOW6432Node\Microsoft\VisualStudio\SxS\VS"
                        ),
                        (
                            winreg.HKEY_LOCAL_MACHINE,
                            r"SOFTWARE\Microsoft\VisualStudio\SxS\VC7"
                        ),
                        (
                            winreg.HKEY_LOCAL_MACHINE,
                            r"SOFTWARE\WOW6432Node\Microsoft\VisualStudio\SxS\VC7"
                        )
                    ]

                    for hkey, subkey in reg_keys:
                        try:
                            with winreg.OpenKey(hkey, subkey) as key:
                                count = winreg.QueryInfoKey(key)[1]

                                for i in range(count):
                                    _, value, _ = winreg.EnumValue(key, i)

                                    if not value:
                                        continue

                                    p = Path(value)

                                    candidates = [
                                        (
                                            p /
                                            "VC" /
                                            "Auxiliary" /
                                            "Build" /
                                            "vcvars64.bat",
                                            ""
                                        ),
                                        (
                                            p /
                                            "VC" /
                                            "Auxiliary" /
                                            "Build" /
                                            "vcvarsall.bat",
                                            "amd64"
                                        ),
                                        (
                                            p /
                                            "Auxiliary" /
                                            "Build" /
                                            "vcvars64.bat",
                                            ""
                                        ),
                                        (
                                            p /
                                            "Auxiliary" /
                                            "Build" /
                                            "vcvarsall.bat",
                                            "amd64"
                                        )
                                    ]

                                    for candidate, args in candidates:
                                        if candidate.is_file():
                                            vcvars_candidates.append(
                                                (candidate, args)
                                            )

                        except OSError:
                            pass

                except (ImportError, AttributeError):
                    pass

                seen_candidates = set()

                for vcvars, vcvars_args in vcvars_candidates:
                    candidate_key = (
                        str(vcvars).lower(),
                        str(vcvars_args).lower()
                    )

                    if candidate_key in seen_candidates:
                        continue

                    seen_candidates.add(candidate_key)

                    environment = None

                    try:
                        with tempfile.NamedTemporaryFile(
                            mode="w",
                            suffix=".bat",
                            delete=False
                        ) as tf:
                            tf.write(
                                "@echo off\r\n"
                                f'call "{vcvars}" {vcvars_args}\r\n'
                                "set\r\n"
                            )
                            temp_bat = tf.name

                        try:
                            result = subprocess.run(
                                [
                                    "cmd.exe",
                                    "/d",
                                    "/c",
                                    temp_bat
                                ],
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                text=True,
                                encoding="mbcs",
                                errors="replace",
                                shell=False
                            )

                            if result.returncode == 0 or (
                                result.stdout and
                                "PATH=" in result.stdout.upper()
                            ):
                                environment = os.environ.copy()

                                for line in result.stdout.splitlines():
                                    if "=" not in line:
                                        continue

                                    k, v = line.split("=", 1)

                                    if k:
                                        environment[k] = v

                        finally:
                            try:
                                os.unlink(temp_bat)
                            except OSError:
                                pass

                    except OSError:
                        pass

                    if environment is None:
                        continue

                    cl_check = shutil.which(
                        "cl.exe",
                        path=environment.get("PATH", "")
                    )

                    if cl_check:
                        (
                            SUPPORTED_FORMAT_VERSIONS
                            .F1_0_A
                            .BUILD
                            ._msvc_environment
                        ) = environment

                        return environment

                if existing_cl:
                    environment = os.environ.copy()
                    environment["PATH"] = (
                        str(Path(existing_cl).parent) +
                        os.pathsep +
                        environment.get("PATH", "")
                    )

                    (
                        SUPPORTED_FORMAT_VERSIONS
                        .F1_0_A
                        .BUILD
                        ._msvc_environment
                    ) = environment

                    return environment

                return None

            @staticmethod
            def _find_c_compiler():
                system = platform.system().lower()

                if system == "windows":
                    msvc_environment = (
                        SUPPORTED_FORMAT_VERSIONS
                        .F1_0_A
                        .BUILD
                        ._find_msvc_environment()
                    )

                    if msvc_environment is not None:
                        cl = shutil.which(
                            "cl.exe",
                            path=msvc_environment.get("PATH", "")
                        )

                        if cl:
                            return cl, "msvc"

                    cl = shutil.which("cl.exe")

                    if cl:
                        return cl, "msvc"

                    return None, None

                if system == "darwin":
                    clang = shutil.which("clang")

                    if clang:
                        return clang, "clang"

                    gcc = shutil.which("gcc")

                    if gcc:
                        return gcc, "gcc"

                    return None, None

                clang = shutil.which("clang")

                if clang:
                    return clang, "clang"

                gcc = shutil.which("gcc")

                if gcc:
                    return gcc, "gcc"

                return None, None

            @staticmethod
            def _find_cpp_compiler():
                system = platform.system().lower()

                if system == "windows":
                    msvc_environment = (
                        SUPPORTED_FORMAT_VERSIONS
                        .F1_0_A
                        .BUILD
                        ._find_msvc_environment()
                    )

                    if msvc_environment is not None:
                        cl = shutil.which(
                            "cl.exe",
                            path=msvc_environment.get("PATH", "")
                        )

                        if cl:
                            return cl, "msvc"

                    cl = shutil.which("cl.exe")

                    if cl:
                        return cl, "msvc"

                    return None, None

                if system == "darwin":
                    clangxx = shutil.which("clang++")

                    if clangxx:
                        return clangxx, "clang++"

                    gxx = shutil.which("g++")

                    if gxx:
                        return gxx, "g++"

                    return None, None

                clangxx = shutil.which("clang++")

                if clangxx:
                    return clangxx, "clang++"

                gxx = shutil.which("g++")

                if gxx:
                    return gxx, "g++"

                return None, None

            @staticmethod
            def _find_wasm_compiler():
                wasi_sysroot = os.environ.get("WASI_SYSROOT")

                compiler_candidates = [
                    ("clang++", "clang++"),
                    ("clang", "clang"),
                    ("wasi-sdk-clang++", "clang++"),
                    ("wasi-sdk-clang", "clang")
                ]

                for executable, compiler_kind in compiler_candidates:
                    compiler = shutil.which(executable)

                    if not compiler:
                        continue

                    sysroot = None

                    if wasi_sysroot:
                        candidate = Path(wasi_sysroot)

                        if candidate.is_dir():
                            sysroot = candidate

                    if sysroot is None:
                        compiler_path = Path(compiler).resolve()

                        candidates = [
                            compiler_path.parent.parent /
                            "share" /
                            "wasi-sysroot",

                            compiler_path.parent.parent.parent /
                            "share" /
                            "wasi-sysroot",

                            compiler_path.parent /
                            "wasi-sysroot"
                        ]

                        for candidate in candidates:
                            if candidate.is_dir():
                                sysroot = candidate
                                break

                    if sysroot is not None:
                        return compiler, compiler_kind, sysroot

                wasi_sdk_path = os.environ.get("WASI_SDK_PATH")

                if wasi_sdk_path:
                    root = Path(wasi_sdk_path)

                    sysroot_candidates = [
                        root / "share" / "wasi-sysroot",
                        root / "sysroot"
                    ]

                    compiler_candidates = [
                        root / "bin" / "clang++.exe",
                        root / "bin" / "clang++",
                        root / "bin" / "clang.exe",
                        root / "bin" / "clang"
                    ]

                    for compiler_path in compiler_candidates:
                        if not compiler_path.is_file():
                            continue

                        for sysroot in sysroot_candidates:
                            if sysroot.is_dir():
                                compiler_kind = (
                                    "clang++"
                                    if "++" in compiler_path.name
                                    else "clang"
                                )

                                return (
                                    str(compiler_path),
                                    compiler_kind,
                                    sysroot
                                )

                return None, None, None

            @staticmethod
            def _gcc_flags(flags):
                result = []

                for flag in flags:
                    flag = str(flag)

                    if flag == "/O2":
                        result.append("-O2")
                    elif flag == "/Ox":
                        result.append("-O3")
                    elif flag.startswith("/std:c++"):
                        result.append(
                            "-std=" + flag[len("/std:"):]
                        )
                    elif flag.startswith("/"):
                        result.append("-" + flag[1:])
                    else:
                        result.append(flag)

                return result

            @staticmethod
            def _write_build_log(log, results):
                log.write_text(
                    "\n\n".join(
                        (
                            f"[{stage}]\n"
                            f"RETURN CODE: {return_code}\n"
                            f"STDOUT:\n{stdout or ''}\n"
                            f"STDERR:\n{stderr or ''}"
                        )
                        for (
                            stage,
                            return_code,
                            stdout,
                            stderr
                        ) in results
                    ),
                    encoding="utf-8"
                )

            @staticmethod
            def _execute_build_stages(
                stages,
                log,
                environment=None
            ):
                results = []

                for stage, command in stages:
                    result = (
                        SUPPORTED_FORMAT_VERSIONS
                        .F1_0_A
                        .BUILD
                        ._run_build_command(
                            command,
                            stage,
                            environment=environment
                        )
                    )

                    results.append(
                        (
                            stage,
                            result.returncode,
                            result.stdout,
                            result.stderr
                        )
                    )

                    if result.returncode != 0:
                        (
                            SUPPORTED_FORMAT_VERSIONS
                            .F1_0_A
                            .BUILD
                            ._write_build_log(
                                log,
                                results
                            )
                        )

                        output = (
                            result.stderr or
                            result.stdout or
                            f"compiler exited with code {result.returncode}"
                        ).strip()

                        raise RuntimeError(
                            f"[BUILD ERROR] {stage} failed\n{output}"
                        )

                (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._write_build_log(
                        log,
                        results
                    )
                )

                return results

            @staticmethod
            def _compiler_environment(compiler_kind):
                if compiler_kind == "msvc":
                    environment = (
                        SUPPORTED_FORMAT_VERSIONS
                        .F1_0_A
                        .BUILD
                        ._find_msvc_environment()
                    )

                    if environment is not None:
                        return environment

                return os.environ.copy()

            @staticmethod
            def _process_c_cpp(item, is_cpp=False):
                platform_name = _get_platform_name()

                if is_cpp:
                    compiler, compiler_kind = (
                        SUPPORTED_FORMAT_VERSIONS
                        .F1_0_A
                        .BUILD
                        ._find_cpp_compiler()
                    )
                else:
                    compiler, compiler_kind = (
                        SUPPORTED_FORMAT_VERSIONS
                        .F1_0_A
                        .BUILD
                        ._find_c_compiler()
                    )

                if compiler is None:
                    if platform.system().lower() == "windows":
                        raise RuntimeError(
                            "[BUILD ERROR] C/C++ compiler not found. "
                            "No supported compiler was found in PATH "
                            "and Visual Studio could not be detected."
                        )

                    raise RuntimeError(
                        "[BUILD ERROR] No supported compiler was found."
                    )

                environment = (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._compiler_environment(
                        compiler_kind
                    )
                )

                inputs = item.get("inputs", [])
                flags = list(item.get("flags", []))

                build_target = str(
                    item.get(
                        "build_target",
                        "executable"
                    )
                ).lower()

                build_type = str(
                    item.get(
                        "build_type",
                        "release"
                    )
                ).lower()

                output_str = resolve_ext(
                    item.get("output", "out"),
                    build_target,
                    platform_name
                )

                output_path = Path(output_str)

                build_dir = (
                    output_path.parent /
                    (
                        "Debug"
                        if build_type == "debug"
                        else "Release"
                    )
                )

                artifact_dir = build_dir / "build_artifacts"

                output_path.parent.mkdir(
                    parents=True,
                    exist_ok=True
                )

                build_dir.mkdir(
                    parents=True,
                    exist_ok=True
                )

                artifact_dir.mkdir(
                    parents=True,
                    exist_ok=True
                )

                log = artifact_dir / "build.log"

                print()
                print(f"  Compiler     : {compiler}")
                print(f"  Compiler type: {compiler_kind}")
                print(f"  Build target : {build_target}")
                print(f"  Build type   : {build_type}")

                objs = []
                static_libs = []
                dynamic_libs = []
                stages = []

                clang_flags = (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._gcc_flags(flags)
                    if compiler_kind != "msvc"
                    else flags
                )

                for inp in inputs:
                    inp_type = str(
                        inp.get("type", "")
                    ).lower()

                    inp_path_str = resolve_ext(
                        inp.get("input", ""),
                        inp_type,
                        platform_name
                    )

                    inp_path = Path(inp_path_str)

                    if inp_type in ("c", "cpp"):
                        if not inp_path.is_file():
                            raise RuntimeError(
                                f"[BUILD ERROR] Source file not found: {inp_path}"
                            )

                        name = inp_path.stem

                        obj = (
                            artifact_dir /
                            (
                                name + ".obj"
                                if compiler_kind == "msvc"
                                else name + ".o"
                            )
                        )

                        objs.append(str(obj))

                        if compiler_kind == "msvc":
                            stages.append(
                                (
                                    f"compile {name}",
                                    [
                                        compiler,
                                        *flags,
                                        "/c",
                                        str(inp_path),
                                        f"/Fo{obj}"
                                    ]
                                )
                            )
                        else:
                            stages.append(
                                (
                                    f"compile {name}",
                                    [
                                        compiler,
                                        *clang_flags,
                                        "-c",
                                        str(inp_path),
                                        "-o",
                                        str(obj)
                                    ]
                                )
                            )

                    elif inp_type == "static":
                        static_libs.append(str(inp_path))

                    elif inp_type == "dynamic":
                        dynamic_libs.append(str(inp_path))

                if build_target == "static":
                    if compiler_kind == "msvc":
                        lib_exe = (
                            shutil.which(
                                "lib.exe",
                                path=environment.get("PATH", "")
                            )
                            or
                            str(
                                Path(compiler).parent /
                                "lib.exe"
                            )
                        )

                        stages.append(
                            (
                                "link static",
                                [
                                    lib_exe,
                                    f"/OUT:{output_path}",
                                    *objs
                                ]
                            )
                        )
                    else:
                        ar = (
                            shutil.which("ar")
                            or
                            shutil.which("llvm-ar")
                        )

                        if ar is None:
                            raise RuntimeError(
                                "[BUILD ERROR] ar/llvm-ar not found."
                            )

                        stages.append(
                            (
                                "link static",
                                [
                                    ar,
                                    "rcs",
                                    str(output_path),
                                    *objs
                                ]
                            )
                        )

                elif build_target == "dynamic":
                    if compiler_kind == "msvc":
                        stages.append(
                            (
                                "link dynamic",
                                [
                                    compiler,
                                    *flags,
                                    "/LD",
                                    f"/Fe{output_path}",
                                    *objs,
                                    *static_libs,
                                    *dynamic_libs
                                ]
                            )
                        )
                    else:
                        stages.append(
                            (
                                "link dynamic",
                                [
                                    compiler,
                                    *clang_flags,
                                    "-shared",
                                    "-o",
                                    str(output_path),
                                    *objs,
                                    *static_libs,
                                    *dynamic_libs
                                ]
                            )
                        )

                else:
                    if compiler_kind == "msvc":
                        stages.append(
                            (
                                "link executable",
                                [
                                    compiler,
                                    *flags,
                                    f"/Fe{output_path}",
                                    *objs,
                                    *static_libs,
                                    *dynamic_libs
                                ]
                            )
                        )
                    else:
                        stages.append(
                            (
                                "link executable",
                                [
                                    compiler,
                                    *clang_flags,
                                    "-o",
                                    str(output_path),
                                    *objs,
                                    *static_libs,
                                    *dynamic_libs
                                ]
                            )
                        )

                (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._execute_build_stages(
                        stages,
                        log,
                        environment
                    )
                )

                return output_path

            @staticmethod
            def build_c(item):
                return (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._process_c_cpp(
                        item,
                        is_cpp=False
                    )
                )

            @staticmethod
            def build_cpp(item):
                return (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._process_c_cpp(
                        item,
                        is_cpp=True
                    )
                )

            @staticmethod
            def build_wasm(item):
                platform_name = _get_platform_name()

                compiler, compiler_kind, sysroot = (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._find_wasm_compiler()
                )

                if compiler is None:
                    raise RuntimeError(
                        "[BUILD ERROR] WASI Clang compiler not found in PATH."
                    )

                if sysroot is None:
                    raise RuntimeError(
                        "[BUILD ERROR] WASI sysroot was not found. "
                        "Set WASI_SYSROOT or WASI_SDK_PATH."
                    )

                inputs = item.get("inputs", [])
                flags = list(item.get("flags", []))

                build_target = str(
                    item.get(
                        "build_target",
                        "executable"
                    )
                ).lower()

                build_type = str(
                    item.get(
                        "build_type",
                        "release"
                    )
                ).lower()

                output_str = resolve_ext(
                    item.get("output", "out"),
                    (
                        "executable"
                        if build_target != "static"
                        else "static"
                    ),
                    platform_name
                )

                if (
                    build_target != "static"
                    and
                    not output_str.endswith(".wasm")
                ):
                    output_str += ".wasm"

                output_path = Path(output_str)

                build_dir = (
                    output_path.parent /
                    (
                        "Debug"
                        if build_type == "debug"
                        else "Release"
                    )
                )

                artifact_dir = build_dir / "build_artifacts"

                output_path.parent.mkdir(
                    parents=True,
                    exist_ok=True
                )

                build_dir.mkdir(
                    parents=True,
                    exist_ok=True
                )

                artifact_dir.mkdir(
                    parents=True,
                    exist_ok=True
                )

                log = artifact_dir / "build.log"

                gcc_flags = (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._gcc_flags(flags)
                )

                common_flags = [
                    "--target=wasm32-wasip1",
                    f"--sysroot={sysroot}",
                    "-D_WASI_EMULATED_SIGNAL",
                    "-D_WASI_EMULATED_PROCESS_CLOCKS",
                    *gcc_flags
                ]

                if compiler_kind == "clang++":
                    common_flags.extend(
                        [
                            "-stdlib=libc++",
                            "-fwasm-exceptions",
                            "-mexception-handling"
                        ]
                    )

                print()
                print(f"  Compiler     : {compiler}")
                print(f"  Compiler type: {compiler_kind}")
                print(f"  WASI sysroot : {sysroot}")
                print(f"  Build type   : {build_type}")

                objs = []
                static_libs = []
                stages = []

                environment = os.environ.copy()

                for inp in inputs:
                    inp_type = str(
                        inp.get("type", "")
                    ).lower()

                    inp_path_str = resolve_ext(
                        inp.get("input", ""),
                        inp_type,
                        platform_name
                    )

                    inp_path = Path(inp_path_str)

                    if inp_type in ("c", "cpp"):
                        if not inp_path.is_file():
                            raise RuntimeError(
                                f"[BUILD ERROR] Source file not found: {inp_path}"
                            )

                        name = inp_path.stem
                        obj = artifact_dir / f"{name}.o"

                        objs.append(str(obj))

                        stages.append(
                            (
                                f"compile {name}",
                                [
                                    compiler,
                                    *common_flags,
                                    "-c",
                                    str(inp_path),
                                    "-o",
                                    str(obj)
                                ]
                            )
                        )

                    elif inp_type == "static":
                        static_libs.append(str(inp_path))

                if build_target == "static":
                    ar = (
                        shutil.which("ar")
                        or
                        shutil.which("llvm-ar")
                    )

                    if ar is None:
                        raise RuntimeError(
                            "[BUILD ERROR] ar/llvm-ar not found."
                        )

                    stages.append(
                        (
                            "link static",
                            [
                                ar,
                                "rcs",
                                str(output_path),
                                *objs
                            ]
                        )
                    )
                else:
                    stages.append(
                        (
                            "link wasm executable",
                            [
                                compiler,
                                *common_flags,
                                *objs,
                                *static_libs,
                                "-lc++abi",
                                "-lwasi-emulated-signal",
                                "-Wl,--export-all",
                                "-Wl,--no-entry",
                                "-o",
                                str(output_path)
                            ]
                        )
                    )

                (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._execute_build_stages(
                        stages,
                        log,
                        environment
                    )
                )

                return output_path

            @staticmethod
            def build_rust(item):
                platform_name = _get_platform_name()

                rustc = shutil.which("rustc")

                if rustc is None:
                    raise RuntimeError(
                        "[BUILD ERROR] rustc not found in PATH."
                    )

                inputs = item.get("inputs", [])
                flags = list(item.get("flags", []))

                build_target = str(
                    item.get(
                        "build_target",
                        "executable"
                    )
                ).lower()

                build_type = str(
                    item.get(
                        "build_type",
                        "release"
                    )
                ).lower()

                output_str = resolve_ext(
                    item.get("output", "out"),
                    build_target,
                    platform_name
                )

                output_path = Path(output_str)

                build_dir = (
                    output_path.parent /
                    (
                        "Debug"
                        if build_type == "debug"
                        else "Release"
                    )
                )

                artifact_dir = build_dir / "build_artifacts"

                output_path.parent.mkdir(
                    parents=True,
                    exist_ok=True
                )

                build_dir.mkdir(
                    parents=True,
                    exist_ok=True
                )

                artifact_dir.mkdir(
                    parents=True,
                    exist_ok=True
                )

                log = artifact_dir / "build.log"

                rust_flags = list(flags)

                if (
                    build_type == "release"
                    and
                    not any(
                        str(flag).startswith("-C")
                        and
                        "opt-level" in str(flag)
                        for flag in rust_flags
                    )
                ):
                    rust_flags.extend(
                        [
                            "-C",
                            "opt-level=3"
                        ]
                    )

                crate_type = "bin"

                if build_target == "dynamic":
                    crate_type = "cdylib"
                elif build_target == "static":
                    crate_type = "staticlib"

                print()
                print(f"  Compiler     : {rustc}")
                print(f"  Build type   : {build_type}")
                print(f"  Crate type   : {crate_type}")

                main_src = None
                libs = []

                for inp in inputs:
                    inp_type = str(
                        inp.get("type", "")
                    ).lower()

                    inp_path_str = resolve_ext(
                        inp.get("input", ""),
                        inp_type,
                        platform_name
                    )

                    if inp_type in ("rs", "rust", "src"):
                        if main_src is None:
                            main_src = inp_path_str

                    elif inp_type in ("static", "dynamic"):
                        libs.append(inp_path_str)

                if main_src is None:
                    raise RuntimeError(
                        "[BUILD ERROR] No main rust source provided."
                    )

                if not Path(main_src).is_file():
                    raise RuntimeError(
                        f"[BUILD ERROR] Rust source file not found: {main_src}"
                    )

                link_args = []

                for lib in libs:
                    link_args.extend(
                        [
                            "-l",
                            Path(lib).stem
                        ]
                    )

                    link_args.extend(
                        [
                            "-L",
                            str(Path(lib).parent)
                        ]
                    )

                stages = [
                    (
                        "build rust",
                        [
                            rustc,
                            main_src,
                            *rust_flags,
                            f"--crate-type={crate_type}",
                            *link_args,
                            "-o",
                            str(output_path)
                        ]
                    )
                ]

                (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .BUILD
                    ._execute_build_stages(
                        stages,
                        log,
                        os.environ.copy()
                    )
                )

                return output_path

        class EXECUTABLE:
            @staticmethod
            def _run_process(command, cwd=None, env=None):
                print()
                print(
                    ANSI.BOLD +
                    ANSI.CYAN +
                    "[EXECUTE]" +
                    ANSI.RESET
                )

                print(
                    f"  Command      : "
                    f"{' '.join(str(x) for x in command)}"
                )

                try:
                    return subprocess.run(
                        command,
                        stdin=None,
                        stdout=None,
                        stderr=None,
                        cwd=cwd,
                        env=env,
                        shell=False
                    )
                except FileNotFoundError as e:
                    raise RuntimeError(
                        "[EXECUTE ERROR] "
                        f"Program or executable file not found: {e}"
                    ) from e
                except OSError as e:
                    raise RuntimeError(
                        "[EXECUTE ERROR] "
                        f"Cannot launch process: {e}"
                    ) from e
                except Exception as e:
                    raise RuntimeError(
                        "[EXECUTE ERROR] "
                        f"Error running process: {e}"
                    ) from e

            @staticmethod
            def _environment():
                return os.environ.copy()

            @staticmethod
            def _dispatch(command, concurrency):
                environment = (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .EXECUTABLE
                    ._environment()
                )

                if not concurrency:
                    result = (
                        SUPPORTED_FORMAT_VERSIONS
                        .F1_0_A
                        .EXECUTABLE
                        ._run_process(
                            command,
                            env=environment
                        )
                    )

                    if result.returncode == 0:
                        print(
                            ANSI.GREEN +
                            "[EXECUTE OK]" +
                            ANSI.RESET
                        )
                    else:
                        print(
                            ANSI.RED +
                            f"[EXECUTE FAILED] "
                            f"Exit code: {result.returncode}" +
                            ANSI.RESET,
                            file=sys.stderr
                        )

                    return result

                def worker():
                    try:
                        result = (
                            SUPPORTED_FORMAT_VERSIONS
                            .F1_0_A
                            .EXECUTABLE
                            ._run_process(
                                command,
                                env=environment
                            )
                        )

                        if result.returncode == 0:
                            print(
                                ANSI.GREEN +
                                "[EXECUTE OK]" +
                                ANSI.RESET
                            )
                        else:
                            print(
                                "[EXECUTE ERROR] "
                                f"Process exited with code "
                                f"{result.returncode}",
                                file=sys.stderr,
                                flush=True
                            )

                    except Exception as e:
                        print(
                            str(e),
                            file=sys.stderr,
                            flush=True
                        )

                thread = threading.Thread(
                    target=worker,
                    daemon=False
                )

                thread.start()

                return thread

            @staticmethod
            def run_python(item):
                platform_name = _get_platform_name()

                path_str = resolve_ext(
                    item["input"],
                    "python",
                    platform_name
                )

                path = Path(path_str)

                concurrency = bool(
                    item.get(
                        "concurency",
                        item.get(
                            "concurrency",
                            False
                        )
                    )
                )

                if not path.is_file():
                    raise RuntimeError(
                        "[EXECUTE ERROR] "
                        f"Python file does not exist: {path}"
                    )

                python = shutil.which("python")

                if python is None:
                    python = shutil.which("python3")

                if python is None:
                    python = sys.executable

                return (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .EXECUTABLE
                    ._dispatch(
                        [
                            python,
                            str(path)
                        ],
                        concurrency
                    )
                )

            @staticmethod
            def run_javascript(item):
                platform_name = _get_platform_name()

                path_str = resolve_ext(
                    item["input"],
                    "javascript",
                    platform_name
                )

                path = Path(path_str)

                concurrency = bool(
                    item.get(
                        "concurency",
                        item.get(
                            "concurrency",
                            False
                        )
                    )
                )

                if not path.is_file():
                    raise RuntimeError(
                        "[EXECUTE ERROR] "
                        f"JavaScript file does not exist: {path}"
                    )

                node = shutil.which("node")

                if node is None:
                    raise RuntimeError(
                        "[EXECUTE ERROR] "
                        "Node.js not found in PATH."
                    )

                return (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .EXECUTABLE
                    ._dispatch(
                        [
                            node,
                            str(path)
                        ],
                        concurrency
                    )
                )

            @staticmethod
            def run_executable(item):
                platform_name = _get_platform_name()

                path_str = resolve_ext(
                    item["input"],
                    "executable",
                    platform_name
                )

                path = Path(path_str)

                concurrency = bool(
                    item.get(
                        "concurency",
                        item.get(
                            "concurrency",
                            False
                        )
                    )
                )

                if not path.is_file():
                    raise RuntimeError(
                        "[EXECUTE ERROR] "
                        f"Executable does not exist: {path}"
                    )

                return (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .EXECUTABLE
                    ._dispatch(
                        [str(path)],
                        concurrency
                    )
                )

            @staticmethod
            def run_bat(item):
                platform_name = _get_platform_name()

                path_str = resolve_ext(
                    item["input"],
                    "bat",
                    platform_name
                )

                path = Path(path_str)

                concurrency = bool(
                    item.get(
                        "concurency",
                        item.get(
                            "concurrency",
                            False
                        )
                    )
                )

                if not path.is_file():
                    raise RuntimeError(
                        "[EXECUTE ERROR] "
                        f"BAT/Shell file does not exist: {path}"
                    )

                if platform_name == "Windows":
                    command = [
                        "cmd.exe",
                        "/c",
                        str(path)
                    ]
                else:
                    command = [
                        "sh",
                        str(path)
                    ]

                return (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .EXECUTABLE
                    ._dispatch(
                        command,
                        concurrency
                    )
                )

            @staticmethod
            def run_ps1(item):
                platform_name = _get_platform_name()

                path_str = resolve_ext(
                    item["input"],
                    "ps1",
                    platform_name
                )

                path = Path(path_str)

                concurrency = bool(
                    item.get(
                        "concurency",
                        item.get(
                            "concurrency",
                            False
                        )
                    )
                )

                if not path.is_file():
                    raise RuntimeError(
                        "[EXECUTE ERROR] "
                        f"PowerShell file does not exist: {path}"
                    )

                powershell = (
                    shutil.which("pwsh")
                    or
                    shutil.which("powershell")
                )

                if powershell is None:
                    raise RuntimeError(
                        "[EXECUTE ERROR] "
                        "PowerShell not found in PATH."
                    )

                return (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .EXECUTABLE
                    ._dispatch(
                        [
                            powershell,
                            "-File",
                            str(path)
                        ],
                        concurrency
                    )
                )

            @staticmethod
            def run_ps2(item):
                return (
                    SUPPORTED_FORMAT_VERSIONS
                    .F1_0_A
                    .EXECUTABLE
                    .run_ps1(item)
                )

def execute(script):
    if not isinstance(script, (list, tuple)):
        print("[EXECUTE ERROR] script must be a list or tuple.", file=sys.stderr, flush=True)
        return False
    success = True
    for index, classed in enumerate(script):
        try:
            if not isinstance(classed, dict): raise RuntimeError(f"[EXECUTE ERROR] Item {index} must be a dict.")
            format_version = classed.get("format_version")
            if format_version is None: raise RuntimeError(f"[EXECUTE ERROR] Item {index} is missing 'format_version'.")
            if format_version != "1.0A": raise RuntimeError(f"[EXECUTE ERROR] Item {index} uses unsupported format_version: {format_version}")
            classes = SUPPORTED_FORMAT_VERSIONS.F1_0_A
            metadata = classed.get("metadata")
            if not isinstance(metadata, dict): raise RuntimeError(f"[EXECUTE ERROR] Item {index} is missing or has invalid type for 'metadata'.")
            item_type = classed.get("type")
            if item_type is None: raise RuntimeError(f"[EXECUTE ERROR] Item {index} is missing 'type'.")
            if item_type == "build":
                build_type = metadata.get("build")
                if build_type is None: raise RuntimeError(f"[BUILD ERROR] Item {index} is missing 'build'.")
                builders = {"c": classes.BUILD.build_c, "cpp": classes.BUILD.build_cpp, "wasm": classes.BUILD.build_wasm, "rust": classes.BUILD.build_rust}
                builder = builders.get(str(build_type).lower())
                if builder is None: raise RuntimeError(f"[BUILD ERROR] Item {index}: unsupported build type: {build_type}")
                builder(metadata)
            elif item_type == "execute":
                executable_type = metadata.get("executable_type")
                if executable_type is None: raise RuntimeError(f"[EXECUTE ERROR] Item {index} is missing 'executable_type'.")
                executors = {"python": classes.EXECUTABLE.run_python, "javascript": classes.EXECUTABLE.run_javascript, "executable": classes.EXECUTABLE.run_executable, "bat": classes.EXECUTABLE.run_bat, "ps1": classes.EXECUTABLE.run_ps1, "ps2": classes.EXECUTABLE.run_ps2}
                executor = executors.get(str(executable_type).lower())
                if executor is None: raise RuntimeError(f"[EXECUTE ERROR] Item {index}: unsupported executable type: {executable_type}")
                result = executor(metadata)
                if isinstance(result, subprocess.CompletedProcess):
                    if result.returncode != 0: raise RuntimeError(f"[EXECUTE ERROR] Item {index} process exited with code {result.returncode}.")
            else:
                raise RuntimeError(f"[EXECUTE ERROR] Item {index}: unsupported type: {item_type}")
        except Exception as e:
            success = False
            print(str(e), file=sys.stderr, flush=True)
    return success

def mono_execute(classed):
    if not isinstance(classed, dict):
        print("[EXECUTE ERROR] classed must be a dict.", file=sys.stderr, flush=True)
        return False
    return execute([classed])

def install_project():
    print()
    print(ANSI.BOLD + ANSI.CYAN + "============================================================" + ANSI.RESET)
    print(ANSI.BOLD + "Install Project" + ANSI.RESET)
    print(ANSI.CYAN + "============================================================" + ANSI.RESET)
    print()

    is_windows = platform.system().lower() == "windows"
    compiler_type = "msvc" if is_windows else "other"
    scripts = BUILD_SCRIPT.get("build", {}).get(compiler_type, [])

    os.makedirs(PROJECT_ROOT, exist_ok=True)
    os.makedirs(INCLUDE_ROOT, exist_ok=True)

    if scripts:
        print(ANSI.GREEN + f"[INSTALL] Executing build scripts for {compiler_type}..." + ANSI.RESET)
        if not execute(scripts):
            print(ANSI.RED + "[INSTALL ERROR] Build execution failed." + ANSI.RESET)
            return False
    else:
        print(ANSI.YELLOW + "[INSTALL] No build scripts found for this compiler." + ANSI.RESET)

    print(ANSI.GREEN + "[INSTALL] Registering executables to global PATH..." + ANSI.RESET)
    global_vars = BUILD_SCRIPT.get("global_variable", [])
    
    if is_windows:
        try:
            import winreg
            key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, "Environment", 0, winreg.KEY_READ | winreg.KEY_WRITE)
            try:
                current_path, _ = winreg.QueryValueEx(key, "PATH")
            except FileNotFoundError:
                current_path = ""
            
            path_list = [p for p in current_path.split(os.pathsep) if p]
            changed = False
            
            for var in global_vars:
                abs_path = str(Path(var).resolve())
                if abs_path not in path_list:
                    path_list.insert(0, abs_path)
                    changed = True
                    print(ANSI.GREEN + f"[INSTALL] Added to Windows Registry PATH: {abs_path}" + ANSI.RESET)
            
            if changed:
                new_path = os.pathsep.join(path_list)
                winreg.SetValueEx(key, "PATH", 0, winreg.REG_EXPAND_SZ, new_path)
                ctypes.windll.user32.SendMessageTimeoutW(0xFFFF, 0x001A, 0, "Environment", 2, 5000, None)
            winreg.CloseKey(key)
        except Exception as e:
            print(ANSI.RED + f"[INSTALL ERROR] Failed to modify Registry: {e}" + ANSI.RESET)
    else:
        home = Path.home()
        profiles = [home / ".bashrc", home / ".zshrc"]
        for var in global_vars:
            abs_path = str(Path(var).resolve())
            export_line = f'export PATH="{abs_path}:$PATH"\n'
            for profile in profiles:
                if profile.exists():
                    content = profile.read_text(encoding="utf-8")
                    if export_line not in content:
                        with profile.open("a", encoding="utf-8") as f:
                            f.write(f"\n{export_line}")
                        print(ANSI.GREEN + f"[INSTALL] Added to {profile.name}: {abs_path}" + ANSI.RESET)
    shutil.copy2("./Program Libraries/registry_editor/registry_editor.h", INCLUDE_ROOT / "registry_editor.h")
    shutil.copy2("./Program Libraries/registry_editor/registry_editor-service-local.h", INCLUDE_ROOT / "registry_editor-service-local.h")
    print(ANSI.BOLD + ANSI.GREEN + "\nInstallation completed successfully." + ANSI.RESET)
    return True

def uninstall_project():
    global ALL

    architecture = get_platform()
    if architecture[1] != 1 or architecture[0] not in (1, 2):
        print(ANSI.RED + "[ERROR] Unsupported system architecture or operating system." + ANSI.RESET)
        return True

    platform_name = _get_platform_name()

    print()
    print(ANSI.BOLD + ANSI.RED + "============================================================" + ANSI.RESET)
    print(ANSI.BOLD + "Uninstall Project" + ANSI.RESET)
    print(f"  Platform     : {platform_name}")
    print(f"  Architecture : {_get_platform_arch()}")
    print(f"  Project Root : {PROJECT_ROOT}")
    print(f"  Include Root : {INCLUDE_ROOT}")
    print(ANSI.RED + "============================================================" + ANSI.RESET)
    print()

    print(ANSI.BOLD + ANSI.YELLOW + "WARNING" + ANSI.RESET)
    print("This action will remove the compiled executables and clean the project directories.")
    print()

    os.makedirs(PROJECT_ROOT, exist_ok=True)
    os.makedirs(INCLUDE_ROOT, exist_ok=True)

    print(ANSI.YELLOW + "[UNINSTALL] Removing executables from PATH..." + ANSI.RESET)

    global_vars = BUILD_SCRIPT.get("global_variable", [])
    is_windows = platform.system().lower() == "windows"

    if is_windows:
        try:
            import winreg

            key = winreg.OpenKey(
                winreg.HKEY_CURRENT_USER,
                "Environment",
                0,
                winreg.KEY_READ | winreg.KEY_WRITE
            )

            try:
                current_path, _ = winreg.QueryValueEx(key, "PATH")
                path_list = [p for p in current_path.split(os.pathsep) if p]

                global_paths = {
                    str(Path(g).resolve()).lower()
                    for g in global_vars
                }

                updated_path_list = [
                    p for p in path_list
                    if str(Path(os.path.expandvars(p)).resolve()).lower() not in global_paths
                ]

                if len(path_list) != len(updated_path_list):
                    new_path = os.pathsep.join(updated_path_list)

                    winreg.SetValueEx(
                        key,
                        "PATH",
                        0,
                        winreg.REG_EXPAND_SZ,
                        new_path
                    )

                    ctypes.windll.user32.SendMessageTimeoutW(
                        0xFFFF,
                        0x001A,
                        0,
                        "Environment",
                        2,
                        5000,
                        None
                    )

                    print(
                        ANSI.GREEN +
                        "[UNINSTALL] Windows Registry PATH cleaned." +
                        ANSI.RESET
                    )

            except FileNotFoundError:
                pass

            winreg.CloseKey(key)

        except Exception as e:
            print(
                ANSI.RED +
                f"[UNINSTALL ERROR] Failed to clean Registry PATH: {e}" +
                ANSI.RESET
            )

    else:
        home = Path.home()
        profiles = [
            home / ".bashrc",
            home / ".zshrc"
        ]

        for var in global_vars:
            abs_path = str(Path(var).resolve())
            export_line = f'export PATH="{abs_path}:$PATH"\n'

            for profile in profiles:
                if profile.exists():
                    content = profile.read_text(encoding="utf-8")

                    if export_line in content:
                        content = content.replace(
                            f"\n{export_line}",
                            ""
                        ).replace(
                            export_line,
                            ""
                        )

                        profile.write_text(
                            content,
                            encoding="utf-8"
                        )

                        print(
                            ANSI.GREEN +
                            f"[UNINSTALL] Removed from {profile.name}: {abs_path}" +
                            ANSI.RESET
                        )

    while True:
        answer = input(" 1:> Yes\n 0:> No\n").strip()

        if answer == "1":
            break

        if answer == "0":
            return True

        print(
            ANSI.RED +
            "Invalid choice. Please try again.\n" +
            ANSI.RESET
        )

    print("\nAnalyzing project directories...")

    os.makedirs(PROJECT_ROOT, exist_ok=True)
    os.makedirs(INCLUDE_ROOT, exist_ok=True)

    roots = [
        PROJECT_ROOT,
        INCLUDE_ROOT
    ]

    total_files = 0
    total_directories = 0

    for root in roots:
        try:
            files = os.listdir(root)
        except FileNotFoundError:
            files = []

        print(f"\n Directory: {root}\n")
        print("Mode          LastWriteTime          Length Name")
        print("----          -------------          ------ ----")

        for name in files:
            path = os.path.join(root, name)
            stat = os.stat(path)

            print(
                f"{get_mode(path):<10} "
                f"{datetime.fromtimestamp(stat.st_mtime):%m/%d/%Y %I:%M %p} "
                f"{'' if stat.st_size == 0 else stat.st_size:>15} "
                f"{name}"
            )

        if len(files) == 0:
            print(
                ANSI.GREEN +
                "\n0 file(s) and directory(ies) found." +
                ANSI.RESET
            )
            continue

        total_files += len(files)

        print(
            "\n",
            len(files),
            " file(s) and directory(ies) found.\n\n"
        )

        for name in files:
            path = os.path.join(root, name)

            print(f"\nDelete {name}")

            try:
                if os.path.isdir(path):
                    shutil.rmtree(path)
                    total_directories += 1
                else:
                    os.remove(path)

                print(
                    ANSI.GREEN +
                    f"[UNINSTALL OK] Deleted {path}" +
                    ANSI.RESET
                )

            except Exception as e:
                print(
                    ANSI.RED +
                    f"[UNINSTALL ERROR] Could not delete {path}: {e}" +
                    ANSI.RESET,
                    file=sys.stderr,
                    flush=True
                )

                continue

    if total_files == 0:
        print(
            ANSI.GREEN +
            "\n0 file(s) and directory(ies) found. Uninstallation complete.\n" +
            ANSI.RESET
        )
        return False

    print(
        ANSI.BOLD +
        ANSI.GREEN +
        "\nUninstallation completed successfully." +
        ANSI.RESET
    )

    return False

def main():
    print(ANSI.BOLD + ANSI.CYAN + "Initializing Solution Builder..." + ANSI.RESET)
    while True:
        match input("\nBuild Mode:\n 0:> Exit\n 1:> Install Project\n 2:> Uninstall Project\nEnter your choice: ").strip():
            case "0":
                print("Exiting...")
                break
            case "1":
                if not install_project(): 
                    input("\nInstallation failed or was cancelled. Press Enter to exit...")
                else: 
                    input("\nPress Enter to exit...")
                break
            case "2":
                if uninstall_project(): 
                    input("\nUninstallation failed or was cancelled. Press Enter to exit...")
                else: 
                    input("\nPress Enter to exit...")
                break
            case _:
                print(ANSI.RED + "Invalid choice. Please try again.\n" + ANSI.RESET)

if __name__ == "__main__":
    main()