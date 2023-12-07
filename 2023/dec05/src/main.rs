fn main() {
    { /* Example 1 */
        print!("Example 1 ");
        let timer = std::time::Instant::now();
        let ans = part1::solution("example-input.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }

    { /* Part 1 */
        print!("Part 1 ");
        let timer = std::time::Instant::now();
        let ans = part1::solution("input.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }

    { /* Example 2 */
        print!("Example 2 ");
        let timer = std::time::Instant::now();
        let ans = part2::solution("example-input.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }

    { /* Part 2 */
        print!("Part 2 ");
        let timer = std::time::Instant::now();
        let ans = part2::solution("input.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }
}

mod part1 {
    pub struct Range {
        pub src_start: u64,
        pub dst_start: u64,
        pub len: u64
    }

    fn parse_range(line: &String) -> Range {
        let vals: Vec<u64> = line.split_ascii_whitespace().map(|s| s.parse().expect("failed to parse")).collect();
        Range { src_start: vals[1], dst_start: vals[0], len: vals[2] }
    }

    pub fn parse_ranges(lines: &Vec<String>, start: &str, end: &str) -> Vec<Range> {
        let start_idx = lines.iter().position(|l| l.starts_with(start)).expect("Invalid start line") + 1;
        let end_idx = lines.iter().position(|l| l.starts_with(end)).or(Some(lines.len())).unwrap() - 1;
        lines[start_idx..end_idx].iter().map(parse_range).collect()
    }

    fn map_range(val: u64, range: &Range) -> Option<u64> {
        if val >= range.src_start && val < range.src_start + range.len {
            let offset = val - range.src_start;
            Some(range.dst_start + offset)
        } else {
            None
        }
    }

    pub fn map_ranges(val: u64, ranges: &Vec<Range>) -> u64 {
        ranges.iter().find_map(|r| map_range(val, r))
            .or(Some(val)).expect("Could not map to range")
    }

    pub fn solution(fpath: &str) -> u64 {
        let lines = utilities::io::read_file_lines(fpath);
        let seeds: Vec<u64> = lines[0].split(":").nth(1).unwrap()
            .split_ascii_whitespace().map(|s| s.parse().unwrap()).collect();

        let seed_to_soil = parse_ranges(&lines, "seed-to-soil", "soil-to-fertilizer");
        let soil_to_fertilizer = parse_ranges(&lines, "soil-to-fertilizer", "fertilizer-to-water");
        let fertilizer_to_water = parse_ranges(&lines, "fertilizer-to-water", "water-to-light");
        let water_to_light = parse_ranges(&lines, "water-to-light", "light-to-temperature");
        let light_to_temperature = parse_ranges(&lines, "light-to-temperature", "temperature-to-humidity");
        let temperature_to_humidity = parse_ranges(&lines, "temperature-to-humidity", "humidity-to-location");
        let humidity_to_location = parse_ranges(&lines, "humidity-to-location", "END OF DOCUMENT");

        seeds.iter().map(|seed| {
            let soil = map_ranges(*seed, &seed_to_soil);
            let fertilizer = map_ranges(soil, &soil_to_fertilizer);
            let water = map_ranges(fertilizer, &fertilizer_to_water);
            let light = map_ranges(water, &water_to_light);
            let temperature = map_ranges(light, &light_to_temperature);
            let humidity = map_ranges(temperature, &temperature_to_humidity);
            map_ranges(humidity, &humidity_to_location)
        }).min().expect("Could not find min location")
    }
}


mod part2 {
    use itertools::Itertools;
    use crate::part1;

    pub fn solution(fpath: &str) -> u64 {
        let lines = utilities::io::read_file_lines(fpath);
        let seed_ranges: Vec<part1::Range> = lines[0].split(":").nth(1).unwrap()
            .split_ascii_whitespace().map(|s| s.parse().unwrap()).into_iter().tuples()
            .map(|(s, l)| part1::Range {src_start: s, dst_start: 0, len: l}).collect_vec();

        let seed_to_soil = part1::parse_ranges(&lines, "seed-to-soil", "soil-to-fertilizer");
        let soil_to_fertilizer = part1::parse_ranges(&lines, "soil-to-fertilizer", "fertilizer-to-water");
        let fertilizer_to_water = part1::parse_ranges(&lines, "fertilizer-to-water", "water-to-light");
        let water_to_light = part1::parse_ranges(&lines, "water-to-light", "light-to-temperature");
        let light_to_temperature = part1::parse_ranges(&lines, "light-to-temperature", "temperature-to-humidity");
        let temperature_to_humidity = part1::parse_ranges(&lines, "temperature-to-humidity", "humidity-to-location");
        let humidity_to_location = part1::parse_ranges(&lines, "humidity-to-location", "END OF DOCUMENT");

        let mut min: Option<u64> = None;
        for seed_range in seed_ranges {
            let end = seed_range.src_start + seed_range.len;
            let tmp_min = (seed_range.src_start..end).into_iter().map(|seed| {
                    let soil = part1::map_ranges(seed, &seed_to_soil);
                    let fertilizer = part1::map_ranges(soil, &soil_to_fertilizer);
                    let water = part1::map_ranges(fertilizer, &fertilizer_to_water);
                    let light = part1::map_ranges(water, &water_to_light);
                    let temperature = part1::map_ranges(light, &light_to_temperature);
                    let humidity = part1::map_ranges(temperature, &temperature_to_humidity);
                    part1::map_ranges(humidity, &humidity_to_location)
                }).min().expect("Could not find min location");
            
            if min.is_none() {
                min = Some(tmp_min);
            } else {
                min = Some(std::cmp::min(min.unwrap(), tmp_min));
            }
        }
        min.unwrap()
    }
}