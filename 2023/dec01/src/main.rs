
fn main() {
    print!("Example 1 ");
    let part1_example_now = std::time::Instant::now();
    let ans_part1_example = part1("part1-example-input.txt");
    println!("({:?}):\t{}", part1_example_now.elapsed(), ans_part1_example);

    print!("Part 1 ");
    let part1_now = std::time::Instant::now();
    let ans_part1 = part1("input.txt");
    println!("({:?}):\t{}", part1_now.elapsed(), ans_part1);

    print!("Example 2 ");
    let part2_example_now = std::time::Instant::now();
    let ans_part2_example = part2("part2-example-input.txt");
    println!("({:?}):\t{}", part2_example_now.elapsed(), ans_part2_example);

    print!("Part 2 ");
    let part2_now = std::time::Instant::now();
    let ans_part2 = part2("input.txt");
    println!("({:?}):\t{}", part2_now.elapsed(), ans_part2);
}

fn part1(input_fpath: &str) -> u32 {
    let lines = utilities::io::read_file_lines(input_fpath);
    let mut calibration_values: Vec<u32> = Vec::new();

    for line in lines {
        let first_digit = find_first_digit_part1(&line);
        let last_digit = find_last_digit_part1(&line);
        let calibration_value = 10 * first_digit + last_digit;
        calibration_values.push(calibration_value);
    }
    
    calibration_values.iter().sum()
}

fn part2(input_fpath: &str) -> u32 {
    let lines = utilities::io::read_file_lines(input_fpath);
    let mut calibration_values: Vec<u32> = Vec::new();

    for line in lines {
        // same as before but use update functions
        let first_digit: u32 = find_first_digit_part2(&line);
        let last_digit: u32 = find_last_digit_part2(&line);
        let calibration_value: u32 = 10 * first_digit + last_digit;
        calibration_values.push(calibration_value);
    }
    
    calibration_values.iter().sum()
}


fn find_first_digit_part1(s: &str) -> u32 {
    let index: usize = s.chars().position(|c| c.is_digit(10)).expect("no digits found");
    s.chars().nth(index).unwrap().to_digit(10).unwrap()
}

fn find_last_digit_part1(s: &str) -> u32 {
    let index: usize = s.chars().rev().position(|c| c.is_digit(10)).expect("no digits found");
    let r_index = s.len() - index - 1;
    s.chars().nth(r_index).unwrap().to_digit(10).unwrap()
}

fn find_first_digit_part2(s: &str) -> u32 {
    let words = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"];

    let real_digit_idx = s.chars().position(|c| c.is_digit(10));
    let mut word_digit: Option<(usize, u32)> = None;

    for (idx, word) in words.iter().enumerate() {
        let substr_find = s.find(word);
        if substr_find.is_some() {
            let found_idx = substr_find.unwrap();
            let digit_value: u32 = (idx + 1).try_into().unwrap();

            if word_digit.is_none() || found_idx < word_digit.unwrap().0 {
                word_digit = Some((found_idx, digit_value));
            }
        }
    }

    if real_digit_idx.is_none() && word_digit.is_none() {
        panic!("Could not find digit")
    } else if word_digit.is_none() {
        s.chars().nth(real_digit_idx.unwrap()).unwrap().to_digit(10).unwrap()
    } else if real_digit_idx.is_none() {
        word_digit.unwrap().1
    } else if real_digit_idx.unwrap() < word_digit.unwrap().0 {
        s.chars().nth(real_digit_idx.unwrap()).unwrap().to_digit(10).unwrap()
    } else {
        word_digit.unwrap().1
    }
}

fn find_last_digit_part2(s: &str) -> u32 {
    let words = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"];

    let real_digit_idx: Option<usize> = s.chars().rev().position(|c| c.is_digit(10));

    let mut word_digit: Option<(usize, u32)> = None;
    for (idx, word) in words.iter().enumerate() {
        let substr_find = s.rfind(word);
        if substr_find.is_some() {
            let found_idx = substr_find.unwrap();
            let digit_value: u32 = (idx + 1).try_into().unwrap();

            if word_digit.is_none() || found_idx > word_digit.unwrap().0 {
                word_digit = Some((found_idx, digit_value));
            }
        }
    }

    let mut rev_digit_idx: Option<usize> = None;
    if real_digit_idx.is_some() {
        rev_digit_idx = Some(s.len() - real_digit_idx.unwrap() - 1);
    }

    if rev_digit_idx.is_none() && word_digit.is_none() {
        panic!("Could not find digit")
    } else if word_digit.is_none() {
        s.chars().nth(rev_digit_idx.unwrap()).unwrap().to_digit(10).unwrap()
    } else if rev_digit_idx.is_none() {
        word_digit.unwrap().1
    } else if rev_digit_idx.unwrap() > word_digit.unwrap().0 {
        s.chars().nth(rev_digit_idx.unwrap()).unwrap().to_digit(10).unwrap()
    } else {
        word_digit.unwrap().1
    }
}