use std::io;

fn main() -> io::Result<()> {
    let mut input = String::new();
    io::stdin().read_line(&mut input);
    let mut previous = input.trim_end().parse::<u32>().unwrap();
    let mut count = 0;
    let mut p2 = 0;
    let mut i : i32 = -2;
    let mut array: [u32; 4] = [previous, 0, 0, 0];
    loop {
        input = String::new();
        match io::stdin().read_line(&mut input) {
            Ok(0) => break,
            Err(_) => break,
            Ok(_) => {
                let next = input.trim_end().parse::<u32>().unwrap();
                if next > previous {
                    count = count + 1;
                }
                previous = next;
                match i {
                    /* 199 => 2 | 200 => 3 */
                    -2 | -1 => array[(i + 4) as usize] = next,
                    0 | 1 | 2 => {
                        array[i as usize] = next;
                        if array[i as usize] > array[(i + 1) as usize] {
                            p2 = p2 + 1;
                        }
                    },
                    3 => {
                        array[3] = next;
                        if array[3] > array[0] {
                            p2 = p2 + 1;
                        }
                    },
                    _ => std::process::exit(1)
                }
                i = i + 1;
                if i == 4 {
                    i = 0;
                }
            }
        }
    }
    println!("{}", count);
    println!("{}", p2);
    Ok(())
}
