use std::io;

fn main() -> io::Result<()> {
    let mut x: i32 = 0;
    let mut y: i32 = 0;
    let mut y2: i32 = 0;
    loop {
        let mut line = String::new();
        match io::stdin().read_line(&mut line) {
            Ok(0) => break,
            Err(_) => break,
            Ok(_) => {
                let v = line.split_whitespace().collect::<Vec<&str>>();
                let value = v[1].parse::<i32>().unwrap();
                match v[0] {
                    "forward" => {
                        x += value;
                        y2 += y * value;
                    },
                    "down" => y += value,
                    "up" => y -= value,
                    _ => break,
                }
            }
        }
    }
    println!("{}", x * y);
    println!("{}", x * y2);
    Ok(())
}
