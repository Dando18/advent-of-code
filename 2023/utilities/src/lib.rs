pub mod io {

    pub fn read_file(path: &str) -> String {
        std::fs::read_to_string(path).expect("Failed to read file")
    }

    pub fn read_file_lines(path: &str) -> Vec<String> {
        read_file(path)
            .lines()
            .map(String::from)
            .collect()
    }
}