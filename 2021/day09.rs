use std::io;

#[derive(Copy, Clone)]
struct Point {
    height: u32,
    marked: bool,
}

fn find_basin(map : &mut Vec::<Vec::<Point>>, r: usize, c: usize) -> usize {
    if map[r][c].marked {
        return 0;
    }
    map[r][c].marked = true;
    if map[r][c].height > 8 {
        return 0;
    }
    return 1
        + find_basin(map, r, c - 1)
        + find_basin(map, r, c + 1)
        + find_basin(map, r - 1, c)
        + find_basin(map, r + 1, c);
}

fn main() -> io::Result<()> {
    let mut map = Vec::<Vec::<Point>>::new();
    let mut input = String::new();
    io::stdin().read_line(&mut input).ok();
    let mut line = input.trim_end();
    let w = line.len() + 2;
    let mut pad = Vec::<Point>::new();
    pad.resize(w, Point{ height : 10, marked : true });
    map.push(pad);
    loop {
        let mut row = Vec::<Point>::new();
        row.reserve(w);
        row.push(Point{ height: 10, marked: true });
        row.extend(line.chars().map(|c|
                                    Point {
                                        height: c as u32 - '0' as u32,
                                        marked: false,
                                    }));
        row.push(Point{ height: 10, marked: true });
        map.push(row);
        input = String::new();
        match io::stdin().read_line(&mut input) {
            Err(_) | Ok(0) | Ok(1) => break,
            Ok(_) => { line = input.trim_end(); },
        }
    }
    let mut pad = Vec::<Point>::new();
    pad.resize(w, Point{ height : 10, marked : true });
    map.push(pad);
    let mut risklevel = 0;
    let mut first = 0;
    let mut second = 0;
    let mut third = 0;
    for r in 1..(map.len() - 1) {
        for c in 1..(w - 1) {
            if map[r][c].height < map[r][c-1].height &&
                map[r][c].height < map[r][c+1].height &&
                map[r][c].height < map[r-1][c].height &&
                map[r][c].height < map[r+1][c].height {
                risklevel += map[r][c].height + 1;
                let nbasin = find_basin(&mut map, r, c);
                if nbasin > first {
                    third = second;
                    second = first;
                    first = nbasin;
                } else if nbasin > second {
                    third = second;
                    second = nbasin;
                } else if nbasin > third {
                    third = nbasin;
                }
            }
        }
    }
    println!("{}", risklevel);
    println!("{}", first * second * third);
    Ok(())
}
