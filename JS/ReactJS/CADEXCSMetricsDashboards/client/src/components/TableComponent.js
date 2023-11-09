import React, {useState} from 'react';
import Paper from '@mui/material/Paper';
import Table from '@mui/material/Table';
import TableBody from '@mui/material/TableBody';
import TableCell from '@mui/material/TableCell';
import TableContainer from '@mui/material/TableContainer';
import TableHead from '@mui/material/TableHead';
import TablePagination from '@mui/material/TablePagination';
import TableRow from '@mui/material/TableRow';

function TableComponent(props){
    const metric = props.metric;
    const data = props.data;

    // *** Pagination
    const [page, setPage] = useState(0);
    const [rowsPerPage, setRowsPerPage] = useState(50);

    const handleChangePage = (event, newPage) => {
        setPage(newPage);
    };

    const handleChangeRowsPerPage = (event) => {
        setRowsPerPage(+event.target.value);
        setPage(0);
    };

    // *** Data    
    function getColumns(metric) {
        let columns = [];
        if(metric === 'SLA'){
            columns = [
                { num: 0, id: 'ticket', label: 'Ticket', minWidth: 100, align: 'left' },
                { num: 1, id: 'dates', label: 'Dates', minWidth: 100, align: 'left' },
            ];
        }

        return columns;
    }

    function getRows(metric){
        let rows = [];
        function createRow(num, ticket, dates, url){
            return { num, ticket, dates, url };
        }

        if(metric === 'SLA'){
            let num = 0;
            for(const ticket of data){
                if(Object.keys(ticket).length !== 0){
                    rows.push(createRow(num, ticket.ticket, ticket.dates, ticket.url));
                    num++;
                }
            }
        }

        return rows;
    }
    
    const dataFormatted = {
        'columns': getColumns(metric),
        'rows': getRows(metric),
    }
    // console.log(dataFormatted);

    function formatValue(value, column, row){
        return (
            <div>
                {(() => {
                    if(metric === 'SLA'){
                        if(column.id === 'ticket'){
                            return (
                                <a className='text-no-decor fnt-bold clr-black' href={row.url}>{row.ticket}</a>
                            )
                        } else if(column.id === 'dates'){
                            return (
                                value.map((item, i) => (
                                    <p key={i}>{item}</p>
                                ))
                            )
                        }
                    }
                })()}
            </div>
        );
    };

    return (
        <div>
            <Paper sx={{maxHeight: 600, width: '100%', overflow: 'hidden' }}>
            <TableContainer sx={{ height: '100%', width: '100%' }}>
                <Table stickyHeader aria-label="sticky table">
                <TableHead sx={{"& th": { backgroundColor: "rgb(168, 143, 202)", fontWeight: "bold", fontSize: "1.25em"}}}>
                    <TableRow>
                        {
                            dataFormatted.columns.map((column) => (
                                <TableCell
                                key={column.num}
                                align={column.align}
                                style={{ minWidth: column.minWidth }}
                                >
                                {column.label}
                                </TableCell>
                            ))
                        }
                    </TableRow>
                </TableHead>
                <TableBody>
                    {dataFormatted.rows
                    .slice(page * rowsPerPage, page * rowsPerPage + rowsPerPage)
                    .map((row) => {
                        return (
                        <TableRow hover role="checkbox" tabIndex={-1} key={row.num}>
                            {
                                dataFormatted.columns.map((column) => {
                                    const value = row[column.id];
                                    return (
                                        <TableCell key={column.num} align={column.align}>{
                                            formatValue(value, column, row)
                                        }
                                        </TableCell>
                                    );
                                })
                            }
                        </TableRow>
                        );
                    })}
                </TableBody>
                </Table>
            </TableContainer>
            <TablePagination
                rowsPerPageOptions={[50, 100]}
                component="div"
                count={dataFormatted.rows.length}
                rowsPerPage={rowsPerPage}
                page={page}
                onPageChange={handleChangePage}
                onRowsPerPageChange={handleChangeRowsPerPage}
            />
            </Paper>
        </div>
    );
}

export { TableComponent }