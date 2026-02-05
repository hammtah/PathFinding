import React, { useState, useCallback } from 'react';
import './App.css';
import { Waves } from 'lucide-react';

const ROWS = 20;
const COLS = 40; // Matching your new 40-col design

const App = () => {
    const startPlaceX = Math.floor(Math.random()*COLS);
    const startPlaceY = Math.floor(Math.random()*ROWS);
    const endPlaceX = Math.floor(Math.random()*COLS);
    const endPlaceY = Math.floor(Math.random()*ROWS);
    const [start, setStart] = useState({ x: startPlaceX, y: startPlaceY });
    const [end, setEnd] = useState({ x: endPlaceX, y: endPlaceY });
    const [obstacles, setObstacles] = useState([]);
    const [path, setPath] = useState([]);
    const [visited, setVisited] = useState([]);
    const [distance, setDistance] = useState(0);
    const [isMouseDown, setIsMouseDown] = useState(false);
    const [loading, setLoading] = useState(false);
    const [editMode, setEditMode] = useState('wall'); // 'start', 'end', or 'wall'
    const [algo, setAlgo] = useState("dij");
    const [swamps, setSwaps] = useState([]);
    const [pathWeight, setPathWeight] = useState(0);//The total weight of the path
    const [visitedWeight, setVisitedWeight] = useState(0);//Total weight of the visited nodes
    // const [data, setData] = useState({});
    const handleCellInteraction = (x, y) => {
        if (editMode === 'start') {
            //Set start/end node if its not an obstacle
            if (!obstacles.some(o => o.x === x && o.y === y)) setStart({ x, y });
        } else if (editMode === 'end') {
            if (!obstacles.some(o => o.x === x && o.y === y)) setEnd({ x, y });
        }else if(editMode === 'swamp'){
            // Toggle swamps
            if ((x === start.x && y === start.y) || (x === end.x && y === end.y)) return;
            setSwaps((prev) => {
                const exists = prev.some(o => o.x === x && o.y === y);
                if (exists) return prev.filter(o => !(o.x === x && o.y === y));
                //Remove from obstacles
                const existsInObstacles = obstacles.some(o => o.x === x && o.y === y);
                if(existsInObstacles) setObstacles(obstacles.filter(o => !(o.x === x && o.y === y)));
                return [...prev, { x, y }];
            });
        }
        else {
            // Toggle Walls
            if ((x === start.x && y === start.y) || (x === end.x && y === end.y)) return;
            setObstacles((prev) => {
                const exists = prev.some(o => o.x === x && o.y === y);
                if (exists) return prev.filter(o => !(o.x === x && o.y === y));
                //Remove from swamps
                //...
                return [...prev, { x, y }];
            });
        }
    };

    const animatePath = (data)=>{
        //Delay cell showing if its a swamp
        //if the current cell is a swamp then add it and delay the showing of the nextCell
        let delay = 0;
        let isSwamp = false;
        for(let i = 0; i < data.path.length; i++) {
            if(isSwamp === true){
                delay += 300;
            }
            isSwamp = swamps.some(s => s.x === data.path[i].x && s.y === data.path[i].y);
            setTimeout(() => {
                setPath(prevState => [...prevState, data.path[i]]);
            }, i*50 + delay);
        }
    }
    const animateVisited =(data)=>{
        for(let i = 0; i < data.visited.length; i++) {
            setTimeout(() => {
                setVisited(prevState => [...prevState, data.visited[i]]);
                if(i===data.visited.length-1){
                    animatePath(data);
                }
            }, i*10);
        }
    }

    const findPath = async () => {
       //in this use case bfs came before dijkstra and astar
        /*
        {
  "width": 40,
  "height": 20,
  "start": {
    "x": 26,
    "y": 19
  },
  "end": {
    "x": 18,
    "y": 9
  },
  "obstacles": [
    {
      "x": 9,
      "y": 17
    },
    {
      "x": 5,
      "y": 14
    },
    {
      "x": 5,
      "y": 11
    },
    {
      "x": 6,
      "y": 9
    },
    {
      "x": 16,
      "y": 11
    },
    {
      "x": 23,
      "y": 12
    },
    {
      "x": 26,
      "y": 11
    },
    {
      "x": 27,
      "y": 10
    },
    {
      "x": 26,
      "y": 8
    },
    {
      "x": 24,
      "y": 7
    },
    {
      "x": 18,
      "y": 7
    },
    {
      "x": 15,
      "y": 8
    },
    {
      "x": 13,
      "y": 8
    },
    {
      "x": 7,
      "y": 9
    },
    {
      "x": 4,
      "y": 8
    },
    {
      "x": 3,
      "y": 7
    },
    {
      "x": 2,
      "y": 7
    },
    {
      "x": 3,
      "y": 4
    },
    {
      "x": 4,
      "y": 3
    },
    {
      "x": 15,
      "y": 3
    },
    {
      "x": 33,
      "y": 2
    },
    {
      "x": 28,
      "y": 2
    },
    {
      "x": 25,
      "y": 2
    },
    {
      "x": 22,
      "y": 2
    },
    {
      "x": 18,
      "y": 2
    },
    {
      "x": 16,
      "y": 3
    },
    {
      "x": 13,
      "y": 4
    },
    {
      "x": 12,
      "y": 5
    },
    {
      "x": 10,
      "y": 6
    },
    {
      "x": 8,
      "y": 7
    },
    {
      "x": 4,
      "y": 9
    },
    {
      "x": 3,
      "y": 9
    },
    {
      "x": 1,
      "y": 9
    },
    {
      "x": 1,
      "y": 8
    },
    {
      "x": 3,
      "y": 6
    },
    {
      "x": 6,
      "y": 5
    },
    {
      "x": 14,
      "y": 5
    },
    {
      "x": 28,
      "y": 6
    },
    {
      "x": 38,
      "y": 6
    },
    {
      "x": 39,
      "y": 8
    },
    {
      "x": 39,
      "y": 9
    },
    {
      "x": 39,
      "y": 12
    },
    {
      "x": 29,
      "y": 15
    },
    {
      "x": 11,
      "y": 14
    },
    {
      "x": 4,
      "y": 14
    },
    {
      "x": 0,
      "y": 15
    },
    {
      "x": 0,
      "y": 16
    },
    {
      "x": 11,
      "y": 17
    },
    {
      "x": 17,
      "y": 17
    },
    {
      "x": 35,
      "y": 17
    },
    {
      "x": 19,
      "y": 17
    },
    {
      "x": 17,
      "y": 18
    },
    {
      "x": 16,
      "y": 18
    },
    {
      "x": 19,
      "y": 18
    },
    {
      "x": 22,
      "y": 17
    },
    {
      "x": 23,
      "y": 16
    },
    {
      "x": 25,
      "y": 14
    },
    {
      "x": 23,
      "y": 14
    },
    {
      "x": 22,
      "y": 15
    },
    {
      "x": 21,
      "y": 15
    },
    {
      "x": 14,
      "y": 14
    },
    {
      "x": 11,
      "y": 13
    },
    {
      "x": 5,
      "y": 13
    },
    {
      "x": 14,
      "y": 10
    },
    {
      "x": 17,
      "y": 7
    },
    {
      "x": 17,
      "y": 6
    },
    {
      "x": 18,
      "y": 6
    },
    {
      "x": 19,
      "y": 6
    },
    {
      "x": 20,
      "y": 7
    },
    {
      "x": 21,
      "y": 8
    },
    {
      "x": 22,
      "y": 10
    },
    {
      "x": 20,
      "y": 12
    },
    {
      "x": 19,
      "y": 12
    },
    {
      "x": 17,
      "y": 10
    },
    {
      "x": 20,
      "y": 5
    },
    {
      "x": 22,
      "y": 4
    },
    {
      "x": 21,
      "y": 3
    },
    {
      "x": 25,
      "y": 0
    },
    {
      "x": 26,
      "y": 3
    },
    {
      "x": 18,
      "y": 4
    },
    {
      "x": 10,
      "y": 1
    },
    {
      "x": 10,
      "y": 0
    },
    {
      "x": 11,
      "y": 0
    },
    {
      "x": 13,
      "y": 0
    },
    {
      "x": 14,
      "y": 0
    },
    {
      "x": 14,
      "y": 1
    },
    {
      "x": 13,
      "y": 1
    },
    {
      "x": 11,
      "y": 2
    },
    {
      "x": 23,
      "y": 0
    },
    {
      "x": 26,
      "y": 2
    },
    {
      "x": 24,
      "y": 1
    },
    {
      "x": 32,
      "y": 0
    },
    {
      "x": 32,
      "y": 2
    },
    {
      "x": 30,
      "y": 3
    },
    {
      "x": 34,
      "y": 4
    },
    {
      "x": 33,
      "y": 8
    },
    {
      "x": 38,
      "y": 1
    },
    {
      "x": 35,
      "y": 4
    },
    {
      "x": 36,
      "y": 4
    },
    {
      "x": 35,
      "y": 8
    },
    {
      "x": 33,
      "y": 9
    },
    {
      "x": 34,
      "y": 9
    },
    {
      "x": 31,
      "y": 16
    },
    {
      "x": 30,
      "y": 15
    },
    {
      "x": 33,
      "y": 10
    },
    {
      "x": 37,
      "y": 10
    },
    {
      "x": 34,
      "y": 14
    },
    {
      "x": 32,
      "y": 13
    },
    {
      "x": 33,
      "y": 11
    },
    {
      "x": 35,
      "y": 11
    },
    {
      "x": 37,
      "y": 15
    },
    {
      "x": 36,
      "y": 15
    },
    {
      "x": 31,
      "y": 9
    },
    {
      "x": 33,
      "y": 6
    },
    {
      "x": 38,
      "y": 8
    },
    {
      "x": 39,
      "y": 17
    },
    {
      "x": 38,
      "y": 18
    },
    {
      "x": 35,
      "y": 19
    },
    {
      "x": 30,
      "y": 18
    },
    {
      "x": 24,
      "y": 12
    },
    {
      "x": 27,
      "y": 7
    },
    {
      "x": 31,
      "y": 7
    },
    {
      "x": 31,
      "y": 10
    },
    {
      "x": 21,
      "y": 16
    },
    {
      "x": 18,
      "y": 13
    },
    {
      "x": 23,
      "y": 17
    },
    {
      "x": 12,
      "y": 16
    },
    {
      "x": 14,
      "y": 13
    },
    {
      "x": 16,
      "y": 12
    },
    {
      "x": 21,
      "y": 11
    },
    {
      "x": 23,
      "y": 11
    },
    {
      "x": 26,
      "y": 12
    },
    {
      "x": 23,
      "y": 18
    },
    {
      "x": 24,
      "y": 16
    },
    {
      "x": 30,
      "y": 17
    },
    {
      "x": 30,
      "y": 19
    },
    {
      "x": 24,
      "y": 19
    },
    {
      "x": 24,
      "y": 18
    },
    {
      "x": 25,
      "y": 17
    },
    {
      "x": 26,
      "y": 17
    },
    {
      "x": 29,
      "y": 18
    },
    {
      "x": 29,
      "y": 19
    },
    {
      "x": 22,
      "y": 19
    },
    {
      "x": 22,
      "y": 18
    },
    {
      "x": 23,
      "y": 19
    },
    {
      "x": 15,
      "y": 19
    },
    {
      "x": 14,
      "y": 18
    },
    {
      "x": 15,
      "y": 17
    },
    {
      "x": 19,
      "y": 16
    },
    {
      "x": 19,
      "y": 19
    },
    {
      "x": 16,
      "y": 19
    },
    {
      "x": 17,
      "y": 16
    },
    {
      "x": 14,
      "y": 19
    },
    {
      "x": 7,
      "y": 18
    },
    {
      "x": 13,
      "y": 17
    },
    {
      "x": 7,
      "y": 19
    },
    {
      "x": 2,
      "y": 18
    },
    {
      "x": 6,
      "y": 16
    },
    {
      "x": 7,
      "y": 16
    },
    {
      "x": 8,
      "y": 16
    },
    {
      "x": 8,
      "y": 17
    },
    {
      "x": 7,
      "y": 17
    },
    {
      "x": 2,
      "y": 15
    },
    {
      "x": 5,
      "y": 12
    },
    {
      "x": 9,
      "y": 11
    },
    {
      "x": 10,
      "y": 13
    },
    {
      "x": 7,
      "y": 15
    },
    {
      "x": 3,
      "y": 14
    },
    {
      "x": 2,
      "y": 13
    },
    {
      "x": 13,
      "y": 9
    },
    {
      "x": 7,
      "y": 12
    },
    {
      "x": 29,
      "y": 10
    },
    {
      "x": 28,
      "y": 11
    },
    {
      "x": 24,
      "y": 11
    },
    {
      "x": 23,
      "y": 10
    },
    {
      "x": 21,
      "y": 9
    },
    {
      "x": 23,
      "y": 6
    },
    {
      "x": 31,
      "y": 4
    },
    {
      "x": 33,
      "y": 5
    },
    {
      "x": 30,
      "y": 4
    },
    {
      "x": 24,
      "y": 3
    },
    {
      "x": 17,
      "y": 3
    },
    {
      "x": 16,
      "y": 5
    },
    {
      "x": 29,
      "y": 2
    },
    {
      "x": 10,
      "y": 5
    },
    {
      "x": 16,
      "y": 2
    },
    {
      "x": 7,
      "y": 1
    },
    {
      "x": 7,
      "y": 3
    },
    {
      "x": 10,
      "y": 3
    },
    {
      "x": 12,
      "y": 2
    },
    {
      "x": 2,
      "y": 0
    },
    {
      "x": 2,
      "y": 1
    },
    {
      "x": 0,
      "y": 1
    },
    {
      "x": 0,
      "y": 3
    },
    {
      "x": 1,
      "y": 3
    },
    {
      "x": 2,
      "y": 3
    },
    {
      "x": 3,
      "y": 3
    },
    {
      "x": 5,
      "y": 1
    },
    {
      "x": 1,
      "y": 2
    },
    {
      "x": 7,
      "y": 2
    },
    {
      "x": 17,
      "y": 0
    },
    {
      "x": 24,
      "y": 0
    },
    {
      "x": 25,
      "y": 4
    },
    {
      "x": 22,
      "y": 7
    }
  ],
  "algo": "astar",
  "swamps": [
    {
      "x": 17,
      "y": 15
    },
    {
      "x": 16,
      "y": 15
    },
    {
      "x": 15,
      "y": 15
    },
    {
      "x": 14,
      "y": 15
    },
    {
      "x": 13,
      "y": 15
    },
    {
      "x": 12,
      "y": 14
    },
    {
      "x": 12,
      "y": 12
    },
    {
      "x": 12,
      "y": 11
    },
    {
      "x": 11,
      "y": 8
    },
    {
      "x": 11,
      "y": 7
    },
    {
      "x": 10,
      "y": 7
    },
    {
      "x": 6,
      "y": 7
    },
    {
      "x": 6,
      "y": 6
    },
    {
      "x": 7,
      "y": 5
    },
    {
      "x": 3,
      "y": 11
    },
    {
      "x": 2,
      "y": 4
    },
    {
      "x": 0,
      "y": 6
    },
    {
      "x": 0,
      "y": 8
    },
    {
      "x": 0,
      "y": 9
    },
    {
      "x": 0,
      "y": 10
    },
    {
      "x": 0,
      "y": 11
    },
    {
      "x": 1,
      "y": 11
    },
    {
      "x": 1,
      "y": 12
    },
    {
      "x": 1,
      "y": 13
    },
    {
      "x": 0,
      "y": 13
    },
    {
      "x": 4,
      "y": 18
    },
    {
      "x": 5,
      "y": 15
    },
    {
      "x": 6,
      "y": 15
    },
    {
      "x": 8,
      "y": 13
    },
    {
      "x": 9,
      "y": 14
    },
    {
      "x": 9,
      "y": 15
    },
    {
      "x": 9,
      "y": 16
    },
    {
      "x": 10,
      "y": 16
    },
    {
      "x": 24,
      "y": 15
    },
    {
      "x": 28,
      "y": 16
    },
    {
      "x": 27,
      "y": 19
    },
    {
      "x": 28,
      "y": 19
    },
    {
      "x": 25,
      "y": 19
    },
    {
      "x": 25,
      "y": 18
    },
    {
      "x": 26,
      "y": 18
    },
    {
      "x": 27,
      "y": 18
    },
    {
      "x": 28,
      "y": 17
    },
    {
      "x": 27,
      "y": 16
    },
    {
      "x": 25,
      "y": 15
    },
    {
      "x": 27,
      "y": 15
    },
    {
      "x": 26,
      "y": 15
    },
    {
      "x": 28,
      "y": 15
    },
    {
      "x": 35,
      "y": 7
    },
    {
      "x": 35,
      "y": 6
    },
    {
      "x": 36,
      "y": 5
    },
    {
      "x": 37,
      "y": 5
    },
    {
      "x": 39,
      "y": 3
    },
    {
      "x": 38,
      "y": 2
    },
    {
      "x": 37,
      "y": 2
    },
    {
      "x": 36,
      "y": 2
    },
    {
      "x": 35,
      "y": 2
    },
    {
      "x": 35,
      "y": 1
    },
    {
      "x": 35,
      "y": 0
    },
    {
      "x": 36,
      "y": 0
    },
    {
      "x": 37,
      "y": 0
    },
    {
      "x": 37,
      "y": 3
    },
    {
      "x": 36,
      "y": 3
    },
    {
      "x": 35,
      "y": 3
    },
    {
      "x": 39,
      "y": 4
    },
    {
      "x": 38,
      "y": 5
    },
    {
      "x": 37,
      "y": 7
    },
    {
      "x": 38,
      "y": 7
    },
    {
      "x": 37,
      "y": 8
    },
    {
      "x": 36,
      "y": 8
    },
    {
      "x": 37,
      "y": 9
    },
    {
      "x": 39,
      "y": 10
    },
    {
      "x": 39,
      "y": 11
    },
    {
      "x": 38,
      "y": 11
    },
    {
      "x": 36,
      "y": 10
    },
    {
      "x": 34,
      "y": 11
    },
    {
      "x": 37,
      "y": 13
    },
    {
      "x": 36,
      "y": 13
    },
    {
      "x": 35,
      "y": 13
    },
    {
      "x": 34,
      "y": 13
    },
    {
      "x": 34,
      "y": 12
    },
    {
      "x": 35,
      "y": 12
    },
    {
      "x": 36,
      "y": 12
    },
    {
      "x": 37,
      "y": 12
    },
    {
      "x": 38,
      "y": 13
    },
    {
      "x": 39,
      "y": 13
    },
    {
      "x": 39,
      "y": 14
    },
    {
      "x": 38,
      "y": 14
    },
    {
      "x": 37,
      "y": 14
    },
    {
      "x": 35,
      "y": 14
    },
    {
      "x": 34,
      "y": 15
    },
    {
      "x": 37,
      "y": 16
    },
    {
      "x": 38,
      "y": 17
    },
    {
      "x": 37,
      "y": 17
    },
    {
      "x": 36,
      "y": 18
    },
    {
      "x": 35,
      "y": 18
    },
    {
      "x": 34,
      "y": 18
    },
    {
      "x": 33,
      "y": 17
    },
    {
      "x": 33,
      "y": 16
    },
    {
      "x": 34,
      "y": 16
    },
    {
      "x": 35,
      "y": 16
    },
    {
      "x": 16,
      "y": 7
    },
    {
      "x": 16,
      "y": 8
    },
    {
      "x": 17,
      "y": 8
    },
    {
      "x": 18,
      "y": 8
    },
    {
      "x": 20,
      "y": 8
    },
    {
      "x": 20,
      "y": 9
    },
    {
      "x": 19,
      "y": 9
    },
    {
      "x": 18,
      "y": 10
    },
    {
      "x": 18,
      "y": 11
    },
    {
      "x": 19,
      "y": 11
    },
    {
      "x": 20,
      "y": 10
    },
    {
      "x": 9,
      "y": 7
    },
    {
      "x": 9,
      "y": 8
    },
    {
      "x": 9,
      "y": 9
    },
    {
      "x": 8,
      "y": 10
    },
    {
      "x": 7,
      "y": 10
    },
    {
      "x": 7,
      "y": 11
    },
    {
      "x": 11,
      "y": 9
    },
    {
      "x": 11,
      "y": 10
    },
    {
      "x": 9,
      "y": 10
    },
    {
      "x": 10,
      "y": 10
    },
    {
      "x": 10,
      "y": 12
    },
    {
      "x": 5,
      "y": 10
    },
    {
      "x": 5,
      "y": 5
    },
    {
      "x": 5,
      "y": 6
    },
    {
      "x": 5,
      "y": 7
    },
    {
      "x": 5,
      "y": 8
    },
    {
      "x": 4,
      "y": 5
    },
    {
      "x": 5,
      "y": 2
    },
    {
      "x": 4,
      "y": 1
    },
    {
      "x": 3,
      "y": 1
    },
    {
      "x": 4,
      "y": 0
    },
    {
      "x": 8,
      "y": 0
    },
    {
      "x": 7,
      "y": 0
    },
    {
      "x": 6,
      "y": 0
    },
    {
      "x": 12,
      "y": 0
    },
    {
      "x": 12,
      "y": 1
    },
    {
      "x": 21,
      "y": 0
    },
    {
      "x": 20,
      "y": 1
    },
    {
      "x": 19,
      "y": 2
    },
    {
      "x": 20,
      "y": 4
    },
    {
      "x": 21,
      "y": 4
    },
    {
      "x": 21,
      "y": 5
    },
    {
      "x": 22,
      "y": 5
    },
    {
      "x": 23,
      "y": 5
    },
    {
      "x": 24,
      "y": 5
    },
    {
      "x": 26,
      "y": 5
    },
    {
      "x": 29,
      "y": 5
    },
    {
      "x": 30,
      "y": 5
    },
    {
      "x": 30,
      "y": 7
    },
    {
      "x": 28,
      "y": 7
    },
    {
      "x": 22,
      "y": 9
    },
    {
      "x": 25,
      "y": 9
    },
    {
      "x": 26,
      "y": 9
    },
    {
      "x": 27,
      "y": 9
    },
    {
      "x": 28,
      "y": 9
    },
    {
      "x": 28,
      "y": 12
    },
    {
      "x": 30,
      "y": 12
    },
    {
      "x": 30,
      "y": 11
    },
    {
      "x": 16,
      "y": 4
    },
    {
      "x": 15,
      "y": 4
    },
    {
      "x": 13,
      "y": 3
    },
    {
      "x": 13,
      "y": 2
    },
    {
      "x": 19,
      "y": 0
    },
    {
      "x": 11,
      "y": 18
    },
    {
      "x": 9,
      "y": 19
    },
    {
      "x": 10,
      "y": 19
    },
    {
      "x": 11,
      "y": 19
    },
    {
      "x": 3,
      "y": 16
    },
    {
      "x": 14,
      "y": 6
    },
    {
      "x": 14,
      "y": 7
    },
    {
      "x": 14,
      "y": 9
    },
    {
      "x": 14,
      "y": 8
    },
    {
      "x": 12,
      "y": 7
    },
    {
      "x": 14,
      "y": 4
    },
    {
      "x": 12,
      "y": 4
    },
    {
      "x": 10,
      "y": 4
    },
    {
      "x": 8,
      "y": 4
    },
    {
      "x": 12,
      "y": 6
    },
    {
      "x": 20,
      "y": 13
    },
    {
      "x": 22,
      "y": 14
    },
    {
      "x": 29,
      "y": 14
    },
    {
      "x": 28,
      "y": 14
    },
    {
      "x": 26,
      "y": 13
    },
    {
      "x": 20,
      "y": 16
    },
    {
      "x": 20,
      "y": 15
    },
    {
      "x": 14,
      "y": 11
    }
  ]
}
        * */
        const req = { "width": COLS, "height": ROWS, "start": start, "end":end, "obstacles":obstacles, "algo": algo, "swamps": swamps };
        console.log(req)
        setLoading(true);
        //Clear previous result
        // setData({});
        setPath([]);
        setDistance(0);
        setVisited([]);
        setVisitedWeight(0);
        setPathWeight(0);
        try {
            const response = await fetch('http://127.0.0.1:18080/api/path', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(req)
            });
            const data = await response.json();
            // setData(data);
            // setPath(data.path || []);
            setPathWeight(data.distance || 0);
            // setVisitedWeight(data.visitedWeight || 0);
            // setVisited(data.visited || []);
            animateVisited(data);

            console.log(data);
        } catch (e) {
            console.error("Pathfinding failed", e);
        } finally {
            setLoading(false);
        }
    };

    return (
        <div className="bg-background-dark font-display text-white min-h-screen flex flex-col overflow-hidden"
             onMouseUp={() => setIsMouseDown(false)}>

            {/* Top Navigation */}
            <header className="flex items-center justify-between border-b border-[#283339] px-6 py-3">
                <div className="flex items-center gap-4">
                    <div className="size-6 text-primary">
                        <svg fill="currentColor" viewBox="0 0 48 48"><path d="M8.578 8.578C5.528 11.628 3.451 15.514 2.609 19.745c-.841 4.23-.409 8.616 1.242 12.601 1.65 3.985 4.446 7.391 8.032 9.788 3.587 2.396 7.804 3.675 12.117 3.675 4.314 0 8.53-1.279 12.117-3.675 3.586-2.397 6.382-5.803 8.033-9.788 1.65-3.985 2.082-8.371 1.241-12.601-.842-4.23-2.919-8.117-5.969-11.167L24 24 8.578 8.578z"/></svg>
                    </div>
                    <h2 className="text-xl font-bold tracking-tight">Pathfinding Visualizer</h2>
                </div>
                <div className="flex gap-3">
                    <button onClick={findPath} disabled={loading} className="min-w-[100px] rounded-lg h-10 px-4 bg-primary text-sm font-bold hover:brightness-110 transition-all">
                        {loading ? '...' : 'Visualize'}
                    </button>
                    <button onClick={() => {setObstacles([]); setPath([]); setVisited([]); setSwaps([]); setPathWeight(0)}} className="min-w-[100px] rounded-lg h-10 px-4 bg-[#283339] text-sm font-bold hover:bg-[#34424a]">
                        Clear Board
                    </button>
                </div>
            </header>

            <div className="flex flex-1 overflow-hidden">
                {/* Sidebar */}
                <aside className="w-72 border-r border-[#283339] flex flex-col p-6 gap-8 ">
                    <div>
                        <h1 className="text-xs font-bold uppercase tracking-widest mb-4 opacity-50">Grid Controls</h1>
                        <div className="flex flex-col gap-1">
                            <ControlBtn active={editMode === 'start'} color="emerald" icon="location_on"
                                        label="Start Node" onClick={() => setEditMode('start')}/>
                            <ControlBtn active={editMode === 'end'} color="rose" icon="flag" label="End Node"
                                        onClick={() => setEditMode('end')}/>
                            <ControlBtn active={editMode === 'wall'} color="slate" icon="edit_square" label="Draw Walls"
                                        onClick={() => setEditMode('wall')}/>
                            <ControlBtn active={editMode === 'swamp'} color="slate" icon="waves" label="Draw Swamps"
                                        onClick={() => setEditMode('swamp')}/>
                            <div>
                                <h1 className="text-white text-xs font-bold uppercase tracking-widest mb-4 opacity-50">Algorithm</h1>
                                <div
                                    className="flex h-12 items-center justify-center rounded-xl bg-[#283339] p-1 gap-1">
                                    <label onClick={()=>setAlgo("bfs")}
                                           className={`flex cursor-pointer h-full grow items-center justify-center rounded-lg px-2 transition-all duration-300  text-[#9db0b9] text-xs font-medium hover:text-white  ${algo == 'bfs'? 'bg-[#111618] font-bold text-white shadow-sm':''}` }>
                                        <span>BFS</span>
                                    </label>
                                    <label onClick={()=>setAlgo("dij")}
                                        className={`flex cursor-pointer h-full grow items-center justify-center rounded-lg px-2 transition-all duration-300  text-[#9db0b9] font-medium text-xs hover:text-white ${algo == 'dij'? 'bg-[#111618] font-bold text-white shadow-sm':''}`}>
                                        <span>Dijkstra</span>
                                    </label>
                                    <label onClick={()=>setAlgo("astar")}
                                        className={`flex cursor-pointer h-full grow items-center justify-center rounded-lg px-2 transition-all duration-300  text-[#9db0b9] text-xs font-medium hover:text-white  ${algo == 'astar'? 'bg-[#111618] font-bold text-white shadow-sm':''}` }>
                                        <span>A* Search</span>
                                    </label>
                                </div>
                            </div>
                        </div>
                    </div>

                    <div className="mt-auto p-4 rounded-xl bg-[#111618] border border-[#283339]">
                        <h4 className="text-sm font-bold mb-2">Algorithm Stats</h4>
                        <div className="flex justify-between text-xs mb-1">
                            <span className="text-[#9db0b9]">Visited Nodes</span>
                            <span className="text-primary font-mono">{(visited.length === 0) ? 0 : (visited.length - 1)}</span>
                        </div>
                        <div className="flex justify-between text-xs mb-1">
                            <span className="text-[#9db0b9]">Shortest Path Nodes</span>
                            <span className="text-primary font-mono">{(path.length === 0) ? 0 : (path.length- 1)}</span>
                        </div>
                        {/*<div className="flex justify-between text-xs">*/}
                        {/*    <span className="text-[#9db0b9]">Visited Nodes Weight</span>*/}
                        {/*    <span className="text-primary font-mono">{visitedWeight}</span>*/}
                        {/*</div>*/}
                        <div className="flex justify-between text-xs ">
                            <span className="text-[#9db0b9]">Path Weight</span>
                            <span className="text-primary font-mono">{pathWeight}</span>
                        </div>
                    </div>
                </aside>

                {/* Main Grid */}
                <main className="flex-1 flex flex-col p-6 overflow-hidden">
                    <div className="flex-1 bg-[#111618] rounded-xl border border-[#283339] p-4 overflow-hidden">
                        <div className="grid h-full w-full" style={{
                            gridTemplateColumns: `repeat(${COLS}, 1fr)`,
                            gridTemplateRows: `repeat(${ROWS}, 1fr)`,
                            gap: '1px'
                        }}>
                            {Array.from({ length: ROWS * COLS }).map((_, idx) => {
                                const x = idx % COLS;
                                const y = Math.floor(idx / COLS);
                                const isStart = start.x === x && start.y === y;
                                const isEnd = end.x === x && end.y === y;
                                const isObstacle = obstacles.some(o => o.x === x && o.y === y);
                                const isSwamp = swamps.some(s => s.x === x && s.y === y);
                                const isPath = path.some(p => p.x === x && p.y === y);
                                const isVisited = visited.some(v => v.x === x && v.y === y);
                                return (
                                    <div
                                        key={idx}
                                        onMouseDown={() => { setIsMouseDown(true); handleCellInteraction(x, y); }}
                                        onMouseEnter={() => { if (isMouseDown) handleCellInteraction(x, y); }}
                                        className={`aspect-square border border-white/5 transition-all duration-300 
                      ${isStart ? 'bg-emerald-500 shadow-[0_0_10px_rgba(16,185,129,0.4)] z-10 scale-95' : ''}
                      ${isEnd ? 'bg-rose-500 shadow-[0_0_10px_rgba(244,63,94,0.4)] z-10 scale-95' : ''}
                      ${isObstacle ? 'bg-[#294559] scale-90 rounded-sm' : ''}
                      ${isSwamp && !isPath ? ' scale-90 rounded-sm swamp' : ''}
                      ${isSwamp && isPath ? 'swamp-path scale-90 rounded-sm ' : ''}
                      ${isPath && !isStart && !isEnd && !isObstacle ? 'bg-primary/90 scale-75' : ''}
                      ${isVisited && !isStart && !isEnd && !isObstacle && !isPath ? 'animate-visited' : ''}
                      ${!isStart && !isEnd && !isObstacle && !isPath && !isVisited && !isSwamp? 'hover:bg-white/5' : ''}
                    `}
                                    >
                                        {isSwamp && (
                                            <Waves
                                                size={14}
                                                strokeWidth={1.5}
                                                color="rgba(255, 255, 255, 0.3)"
                                                className={"flowing-wave"}
                                            />
                                        )}
                                    </div>
                                );
                            })}
                        </div>
                    </div>
                </main>
            </div>
        </div>
    );
};

const ControlBtn = ({ active, color, icon, label, onClick }) => (
    <div onClick={onClick} className={`flex items-center gap-3 px-3 py-3 rounded-lg cursor-pointer transition-colors 
    ${active ? `bg-${color}-500/20 text-${color}-400 border border-${color}-500/30` : 'text-slate-400 hover:bg-slate-500/10'}`}>
        <span className="material-symbols-outlined text-[20px]">{icon}</span>
        <p className="text-sm font-medium">{label}</p>
    </div>
);

export default App;