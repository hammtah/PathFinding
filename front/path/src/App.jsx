import React, { useState, useCallback } from 'react';
import './App.css';

const ROWS = 20;
const COLS = 40; // Matching your new 40-col design

const App = () => {
    const [start, setStart] = useState({ x: 0, y: 0 });
    const [end, setEnd] = useState({ x: COLS-1, y: ROWS-1 });
    const [obstacles, setObstacles] = useState([]);
    const [path, setPath] = useState([]);
    const [visited, setVisited] = useState([]);
    const [distance, setDistance] = useState(0);
    const [isMouseDown, setIsMouseDown] = useState(false);
    const [loading, setLoading] = useState(false);
    const [editMode, setEditMode] = useState('wall'); // 'start', 'end', or 'wall'
    const [algo, setAlgo] = useState("dij");
    // const [data, setData] = useState({});
    const handleCellInteraction = (x, y) => {
        if (editMode === 'start') {
            //Set start/end node if its not an obstacle
            if (!obstacles.some(o => o.x === x && o.y === y)) setStart({ x, y });
        } else if (editMode === 'end') {
            if (!obstacles.some(o => o.x === x && o.y === y)) setEnd({ x, y });
        } else {
            // Toggle Walls
            if ((x === start.x && y === start.y) || (x === end.x && y === end.y)) return;
            setObstacles((prev) => {
                const exists = prev.some(o => o.x === x && o.y === y);
                if (exists) return prev.filter(o => !(o.x === x && o.y === y));
                return [...prev, { x, y }];
            });
        }
    };

    const animatePath = (data)=>{
        for(let i = 0; i < data.path.length; i++) {
            setTimeout(() => {
                setPath(prevState => [...prevState, data.path[i]]);
            }, i*50);
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
        console.log(JSON.stringify({ "width": COLS, "height": ROWS, "start": start, "end":end, "obstacles":obstacles, "algo": algo }))
        setLoading(true);
        // setData({});
        setPath([]);
        setDistance(0);
        setVisited([]);
        try {
            const response = await fetch('http://127.0.0.1:18080/api/path', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ "width": COLS, "height": ROWS, "start": start, "end":end, "obstacles":obstacles, "algo": algo })
            });
            const data = await response.json();
            // setData(data);
            // setPath(data.path || []);
            setDistance(data.distance || 0);
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
                    <button onClick={() => {setObstacles([]); setPath([]); setVisited([])}} className="min-w-[100px] rounded-lg h-10 px-4 bg-[#283339] text-sm font-bold hover:bg-[#34424a]">
                        Clear Board
                    </button>
                </div>
            </header>

            <div className="flex flex-1 overflow-hidden">
                {/* Sidebar */}
                <aside className="w-72 border-r border-[#283339] flex flex-col p-6 gap-8">
                    <div>
                        <h1 className="text-xs font-bold uppercase tracking-widest mb-4 opacity-50">Grid Controls</h1>
                        <div className="flex flex-col gap-1">
                            <ControlBtn active={editMode === 'start'} color="emerald" icon="location_on"
                                        label="Start Node" onClick={() => setEditMode('start')}/>
                            <ControlBtn active={editMode === 'end'} color="rose" icon="flag" label="End Node"
                                        onClick={() => setEditMode('end')}/>
                            <ControlBtn active={editMode === 'wall'} color="slate" icon="edit_square" label="Draw Walls"
                                        onClick={() => setEditMode('wall')}/>
                            <div>
                                <h1 className="text-white text-xs font-bold uppercase tracking-widest mb-4 opacity-50">Algorithm</h1>
                                <div
                                    className="flex h-12 items-center justify-center rounded-xl bg-[#283339] p-1 gap-1">
                                    <label onClick={()=>setAlgo("dij")}
                                        className={`flex cursor-pointer h-full grow items-center justify-center rounded-lg px-2 transition-all duration-300  text-[#9db0b9] font-medium text-xs hover:text-white ${algo == 'dij'? 'bg-[#111618] font-bold text-white shadow-sm':''}`}>
                                        <span>Dijkstra</span>
                                    </label>
                                    <label onClick={()=>setAlgo("bfs")}
                                        className={`flex cursor-pointer h-full grow items-center justify-center rounded-lg px-2 transition-all duration-300  text-[#9db0b9] text-xs font-medium hover:text-white  ${algo == 'bfs'? 'bg-[#111618] font-bold text-white shadow-sm':''}` }>
                                        <span>BFS</span>
                                    </label>
                                    <label onClick={()=>setAlgo("a")}
                                        className="flex cursor-pointer h-full grow items-center justify-center rounded-lg px-2 transition-all duration-300  text-[#9db0b9] text-xs font-medium hover:text-white ">
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
                            <span className="text-primary font-mono">{visited.length}</span>
                        </div>
                        <div className="flex justify-between text-xs">
                            <span className="text-[#9db0b9]">Distance</span>
                            <span className="text-primary font-mono">{path.length}</span>
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
                      ${isPath && !isStart && !isEnd && !isObstacle ? 'bg-primary/90 scale-75' : ''}
                      ${isVisited && !isStart && !isEnd && !isObstacle &&!isPath ? 'animate-visited' : ''}
                      ${!isStart && !isEnd && !isObstacle && !isPath && !isVisited ? 'hover:bg-white/5' : ''}
                    `}
                                    />
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