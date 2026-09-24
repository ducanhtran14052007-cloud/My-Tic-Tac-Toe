import os
import subprocess
import argparse


def run_grader():
    # argument parser
    parser = argparse.ArgumentParser(description="Proeject Grader")
    parser.add_argument(
        "--target",
        type=str,
        default="game",
        help="object filename (default: game)"
    )
    parser.add_argument(
        "--testcase_dir",
        type=str,
        default="../testcase/",
        help="path to testcase directory (default: ../testcase/)"
    )
    args = parser.parse_args()

    # config
    game = args.target
    executable = f"{game}.exe" if os.name == 'nt' else f"./{game}"
    testcase_dir = os.path.abspath(args.testcase_dir)

    # check if executable exist
    if not os.path.exists(executable):
        print(f"[WARNING]: Cannot found '{executable}'!")
        print(f"Please run complie c++ your submit code before running grader.py")
        return

    # Get list of testcase
    try:
        input_files = sorted([f for f in os.listdir(
            testcase_dir) if f.startswith("input")])
    except FileNotFoundError:
        print(f"[ERROR] Cannot found testcase folder at '{testcase_dir}'")
        return

    total_tests = len(input_files)
    passed_tests = 0
    failed_cases = []

    print(f"Grading start for {game}...")
    print("-" * 50)

    for i, file_name in enumerate(input_files, 1):
        input_path = os.path.join(testcase_dir, file_name)

        print(f"[{i}/{total_tests}] Testing: {file_name}...",
              end=" ", flush=True)

        try:
            # Run game
            result = subprocess.run(
                [executable, "-j", "-i", input_path],
                capture_output=True,
                text=True,
                timeout=10  # avoid Infinite Loop
            )

            # check exit code
            if result.returncode == 0:
                ans_file_name = file_name.replace("input", "output")
                ans_path = os.path.join(testcase_dir, ans_file_name)

                if not os.path.exists(ans_path):
                    print(f"FAILED (Missing {ans_file_name})")
                    failed_cases.append(file_name)
                    break

                with open(ans_path, "r") as f:
                    expected_output = f.read().strip()

                actual_output = result.stdout.strip()

                if actual_output == expected_output:
                    print("PASSED")
                    passed_tests += 1

                else:
                    print("FAILED (Wrong Answer)")
                    print(f"\n--- DIFF ({file_name}) ---")
                    print(f"Expected: {expected_output[:100]}...")
                    print(f"Actual:   {actual_output[:100]}...")
                    print("-" * 30)
                    failed_cases.append(file_name)
                    break

            else:
                print("FAILED (Non-zero exit code)")
                print(f"\n--- ERROR LOG ({file_name}) ---")
                print(
                    result.stderr if result.stderr else "Found stderr is empty. Please check log.txt and verify your code logic.")
                print("-" * 30)
                failed_cases.append(file_name)
                break  # stop grading

        except subprocess.TimeoutExpired:
            print(f"TIMEOUT (testcase run pass {10}s)")
            failed_cases.append(file_name)
            break

        except Exception as e:
            print(f"ERROR: {str(e)}")
            failed_cases.append(file_name)
            break

    # overall
    print("-" * 50)
    print("GRADING OVERALL:")
    print(f"- Total testcase: {total_tests}")
    print(f"- Corrects: {passed_tests}")

    percentage = (passed_tests / total_tests) * 100 if total_tests > 0 else 0
    print(f"- Accuracy:  {percentage:.2f}%")

    if failed_cases:
        print(f"- Incorrects: {', '.join(failed_cases)}")
        print("\nGrading process has stopped due to this error testcase.")
    else:
        print("Good job! You've passed all testcase.")


if __name__ == "__main__":
    run_grader()


# # Version 0
# def run_game(game, input_file_path):
#     result = subprocess.run(
#         [f"{game}.exe", "-j", "-i", input_file_path],
#         check=True,
#         capture_output=True,
#         text=True
#     )

#     print(f"Output: {result.stdout}")
#     print(f"Exit Code: {result.returncode}")


# input_file_paths = [os.path.join(testcase_dir, file_path)
#                     for file_path in os.listdir(testcase_dir)
#                     if file_path.startswith("input")]

# print(input_file_paths)
# print(len(input_file_paths))

# for input_file_path in input_file_paths:
#     run_game(game, input_file_path)
